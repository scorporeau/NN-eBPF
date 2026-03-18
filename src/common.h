#ifndef COMMON_H
#define COMMON_H

//common objects

struct flow_attribute
{
    //Those are the 6 "best" features used to do IDS, according to [1]
    s64 num_packet;        // Number of packets
    s64 min_packet_length; // Minimum length of a packet
    s64 max_packet_length; // Maximum length of a packet
    s64 max_duration;      // Maximum time between two packets sent in the forward direction
    s64 dst_port;          // Destination port
    s64 header_length;     // TCP total header length

    s64 last_packet_time;
    s64 status;
    u64 total_feature_extraction_time;
    u64 detection_start_time;

    //id of the current NN parameters
    s32 nn_idx;

    //NN parameters maps
    s32 hidden1[32];
    s32 hidden2[32];
};

struct flow
{
    int saddr;
    int sport;
    int daddr;
    int dport;
};

#define MAX_PACKET_RECORD 8192

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, MAX_PACKET_RECORD);
    __type(key, struct flow);
    __type(value, struct flow_attribute);
} flow_map SEC(".maps");


#endif