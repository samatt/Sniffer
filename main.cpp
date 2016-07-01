#include <iostream>
#include <set>
#include <string>
#include "tins/tins.h"
#include <signal.h>

using namespace Tins;
using namespace std;

void addRadioHeader(stringstream& ss, const RadioTap& rf,Timestamp& t){
    ss<<to_string(t.seconds())<<","<<"Radio,"<<std::to_string((rf.dbm_signal()))<<","<<rf.channel_freq()<<","<<rf.channel_type()<<",";
}
bool processPacket(PDU &pdu, Timestamp& t) {

    const Dot11 &d11 = pdu.rfind_pdu<Dot11>();
    const RadioTap &rf =  pdu.rfind_pdu<RadioTap>();
    stringstream ss;
    ss<<""<<endl;
    Packet p  = Packet();


    if (d11.matches_flag(Tins::PDU::DOT11_MANAGEMENT)) {

        const Dot11ManagementFrame &d11_b = pdu.rfind_pdu<Dot11ManagementFrame>();

        if(d11_b.matches_flag(Tins::PDU::PDUType::DOT11_BEACON)){
            addRadioHeader(ss, rf,t);
            try{
                ss<<"Beacn,"<<d11_b.addr2()<<","<<d11_b.ssid();
            }
            catch(option_not_found){
            }
        }
        else if(d11.matches_flag(Tins::PDU::PDUType::DOT11_PROBE_REQ) ){
            addRadioHeader(ss, rf,t);
            ss<<"Probe," <<d11_b.addr2()<<","<<d11_b.ssid();
        }
    }
    else if (d11.matches_flag(Tins::PDU::DOT11_DATA)){
        const Dot11Data &data = pdu.rfind_pdu<Dot11Data>();
        if (data.to_ds() == 0 && data.from_ds() == 1) {

            if(data.addr2() == data.addr3()){
                addRadioHeader(ss, rf,t);
                ss<<"Data,"<<data.addr1()<<","<<data.addr2();
            }
        }
    }
    if(ss.str().length()>6){
        cout<< ss.str();

    }
    return true;
}

int main(int argc, char* argv[]) {

    std::string interface = "en0";
    if(argc == 2){
        interface = argv[1];
    }

    Sniffer sniffer(interface, 2000, true, "type mgt or type data", true);
    while(Packet pkt = sniffer.next_packet()) {
        Timestamp t=pkt.timestamp();
        processPacket(*pkt.pdu(),t);
    }
}
