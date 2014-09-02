#include <QDebug>
#include <QMapIterator>
#include "memorysystem.h"
#include "utilities.h"

using namespace DRAMSim;

double MemorySystem::m_powerSum[4];
uint64_t MemorySystem::m_powerSamples;

MemorySystem::MemorySystem(sc_module_name name, QMap<QString, QString> configOverrides) : sc_module(name)
{
    m_dramPartFile = "ini/DDR2_micron_16M_8b_x8_sg3E.ini";
    m_systemConfigFile = "ini/system.ini";
    m_megsOfMemory = 512;

    // TODO set MEMC_CLOCK_CYCLE according to selected DRAM part - set to constant 3 ns for now

    // convert the override map to std::map
    QMapIterator<QString, QString> it(configOverrides);
    IniReader::OverrideMap overrideStdMap;

    while(it.hasNext())
    {
        it.next();
        overrideStdMap[it.key().toStdString()] = it.value().toStdString();
    }

    // create the DRAMSim2 memory system instance
    m_DRAMSim = getMemorySystemInstance(m_dramPartFile.toStdString(), m_systemConfigFile.toStdString(),
                                        "/home/maltanar/Desktop/spmv-ocm-sim", "example_app", m_megsOfMemory, NULL, &overrideStdMap);

    m_readCB = new Callback<MemorySystem, void, unsigned, uint64_t, uint64_t>(this, &MemorySystem::readComplete);
    m_writeCB = new Callback<MemorySystem, void, unsigned, uint64_t, uint64_t>(this, &MemorySystem::writeComplete);

    // register callback functions
    // m_DRAMSim->RegisterCallbacks(m_readCB, m_writeCB, NULL);
    // TODO re-enable write callback if desired
    m_DRAMSim->RegisterCallbacks(m_readCB, NULL, &powerCallback);

    // TODO set correct CPU speed here
    // 0 assumes memory controller and CPU running at the same clock speed
    // right now we assume 100 MHz PEs
    m_DRAMSim->setCPUClockSpeed(PE_TICKS_PER_SECOND);

    // declare the runMemorySystem functions as a SystemC thread
    SC_THREAD(runMemorySystem);


    // statistics-related variables
    m_latencySamples = 0;
    m_latencySum = 0;
    m_powerSamples = 0;
    m_powerSum[0] = m_powerSum[1] = m_powerSum[2] = m_powerSum[3] = 0;
}

MemorySystem::~MemorySystem()
{
    delete m_DRAMSim;
    delete m_readCB;
    delete m_writeCB;
}

void MemorySystem::setRequestFIFO(sc_fifo<MemoryOperation *> *fifo)
{
    m_requests = fifo;
}

void MemorySystem::setResponseFIFO(int originID, sc_fifo<MemoryOperation *> *fifo)
{
    m_responseFIFOs[originID] = fifo;
}

void MemorySystem::readComplete(unsigned id, uint64_t address, uint64_t clock_cycle)
{
    // hacky lazy way to return the transaction obj, but oh well...
    Transaction * t = (Transaction *) address;
    // retrieve the corresponding MemoryOperation
    if(!m_reqsInFlight.contains(t))
    {
        qDebug() << "error: Transaction has no matching MemoryOperation!";
        exit(EXIT_FAILURE);
        return;
    }
    MemoryOperation * op = m_reqsInFlight[t];

    // delete the map entry
    m_reqsInFlight.remove(t);


    op->isResponse = true;
    // TODO return real data to the requesters?

    // add response to the appropriate FIFO
    if(!m_responseFIFOs.contains(op->origin))
    {
        qDebug() << "error: Cannot find response FIFO for origin ID " << op->origin;
        exit(EXIT_FAILURE);
    }

    if(!m_responseFIFOs[op->origin]->nb_write(op))
    {
        qDebug() << "error: Response FIFO is full for origin ID " << op->origin;
        exit(EXIT_FAILURE);
    }

    // add latency info to MemoryOperation and statistics
    op->latency = (clock_cycle - t->timeAdded) * MEMC_CLOCK_CYCLE;
    m_latencySum += (clock_cycle - t->timeAdded);
    m_latencySamples++;

    // DRAMsim will deallocate the Transaction
    // the originating PE will deallocate the MemoryOperation
}

void MemorySystem::writeComplete(unsigned id, uint64_t address, uint64_t clock_cycle)
{
    // TODO DRAMsim doesn't seem to keep the write transaction objects intact,
    // so we'll have to handle these differently.
}

void MemorySystem::powerCallback(double background, double burst, double refresh, double actpre)
{
    //qDebug() << "power callback " << background << burst << refresh << actpre;

    m_powerSum[0] += background;
    m_powerSum[1] += burst;
    m_powerSum[2] += refresh;
    m_powerSum[3] += actpre;
    m_powerSamples++;
}

double MemorySystem::getAveragePowerBackground()
{
    return m_powerSum[0] / (double) m_powerSamples;
}

double MemorySystem::getAveragePowerBurst()
{
    return m_powerSum[1] / (double) m_powerSamples;
}

double MemorySystem::getAveragePowerRefresh()
{
    return m_powerSum[2] / (double) m_powerSamples;
}

double MemorySystem::getAveragePowerActPre()
{
    return m_powerSum[3] / (double) m_powerSamples;
}

double MemorySystem::getAverageReqRespLatency()
{
    return (double) m_latencySum / (double) m_latencySamples;
}

void MemorySystem::runMemorySystem()
{
    while(1)
    {
        // this loop means that up to TRANS_QUEUE_DEPTH (e.g 32) requests per memory system cycle
        // will be entered into the DRAM transaction queue
        while(m_requests->num_available() > 0 && m_DRAMSim->willAcceptTransaction())
        {
            // pop the operation from the FIFO
            MemoryOperation * op = m_requests->read();
            Transaction * trans = new Transaction(op->isWrite ? DATA_WRITE : DATA_READ,
                                                  op->address, NULL);

            // add transaction to DRAMsim's queue
            m_DRAMSim->addTransaction(trans);

            if(!op->isWrite)
            {
                // add a mapping entry between the DRAMsim transaction and our memory operation
                m_reqsInFlight.insert(trans, op);
            }
        }

        // advance SystemC time by 1 cc
        wait(PE_CLOCK_CYCLE);
        // advance DRAMSim simulation time by 1 cc
        m_DRAMSim->update();
    }
}
