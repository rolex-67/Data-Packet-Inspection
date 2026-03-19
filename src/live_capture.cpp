#include "live_capture.h"
#include <pcap.h>
#include <iostream>

namespace DPI {

bool LiveCapture::start(PacketCallback callback) {
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* handle = pcap_open_live(
        interface_.c_str(),
        65536,
        1,      // promiscuous mode
        1000,
        errbuf
    );

    if (!handle) {
        std::cerr << "Error opening device: " << errbuf << "\n";
        return false;
    }

    running_ = true;

    while (running_) {
        struct pcap_pkthdr* header;
        const u_char* data;

        int res = pcap_next_ex(handle, &header, &data);

        if (res == 1) {
            PacketJob job;

            job.data.assign(data, data + header->len);
            job.ts_sec = header->ts.tv_sec;
            job.ts_usec = header->ts.tv_usec;

            callback(job);
        }
    }

    pcap_close(handle);
    return true;
}

void LiveCapture::stop() {
    running_ = false;
}

}