// EPOS Ethernet Mediator Common Package

#include <nic.h>

#ifndef __ethernet_h
#define __ethernet_h

#include <utility/string.h>

__BEGIN_SYS

class Ethernet: private NIC_Common
{
protected:
    Ethernet() {}

public:
    static const unsigned int MTU = 1500;
    static const unsigned int HEADER_SIZE = 14;

    typedef NIC_Common::Address<6> Address;
    typedef char PDU[MTU];
    typedef NIC_Common::CRC32 CRC;

    typedef unsigned short Protocol;
    enum
    {
        ARP    = 0x0806,
        RARP   = 0x8035,
        ELP    = 0x8888,
        IP     = 0x0800,
        ROUTER = 0x0801
    };

    // The Ethernet Frame (RFC 894)
    class Frame
    {
    public:
        Frame(const Address & src, const Address & dst, const Protocol & prot)
        : _dst(dst), _src(src), _prot(prot) { }

        Frame(const Address & src, const Address & dst, const Protocol & prot, const void * data, unsigned int size)
        : _dst(dst), _src(src), _prot(prot)
        {
            memcpy(_data, data, size);
        }
        
        friend OStream & operator << (OStream & db, const Frame & f) {
            db << "{" << Address(f._dst)
               << "," << Address(f._src)
               << "," << f._prot
               << "," << f._data << "}";
            return db;
        }
        
    public:
        Address _dst;
        Address _src;
        Protocol _prot;
        PDU _data;
        CRC _crc;
    };

    // Meaningful statistics for Ethernet
    struct Statistics: public NIC_Common::Statistics
    {
        Statistics() : rx_overruns(0), tx_overruns(0), frame_errors(0),
        	       carrier_errors(0), collisions(0) {}

        friend OStream & operator << (OStream & db, const Statistics & s) {
            db << "{rxp=" << s.rx_packets
               << ",rxb=" <<  s.rx_bytes
               << ",rxorun=" <<  s.rx_overruns
               << ",txp=" <<  s.tx_packets
               << ",txb=" <<  s.tx_bytes
               << ",txorun=" <<  s.tx_overruns
               << ",frm=" <<  s.frame_errors
               << ",car=" <<  s.carrier_errors
               << ",col=" <<  s.collisions
               << "}";
            return db;
        }
        
        unsigned int rx_overruns;
        unsigned int tx_overruns;
        unsigned int frame_errors;
        unsigned int carrier_errors;
        unsigned int collisions;
    };

    static const Address BROADCAST;
};

__END_SYS

#endif

