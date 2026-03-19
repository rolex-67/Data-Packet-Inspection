# 🚀 DPI Engine - Deep Packet Inspection System (Student Version)

This project provides a comprehensive look at how a **Deep Packet Inspection (DPI)** system works—bridging the gap between basic networking theory and a high-performance implementation in C++.

### 🎯 Learning Objectives
After exploring this project, you will be able to:
* **Understand** how network packets are structured at a granular level.
* **Explain** how DPI identifies traffic even within encrypted HTTPS streams.
* **Describe** the lifecycle of a packet through a multi-threaded system.
* **Discuss** these concepts confidently in technical engineering interviews.

---

## 📚 Table of Contents
1. [What is DPI?](#1-what-is-dpi)
2. [Networking Basics](#2-networking-basics)
3. [Project Overview](#3-project-overview)
4. [File Structure](#4-file-structure)
5. [Packet Flow (Simple)](#5-packet-flow-simple-version)
6. [Multi-threaded Architecture](#6-multi-threaded-version)
7. [Core Components](#7-core-components)
8. [SNI Extraction](#8-sni-extraction)
9. [Blocking Logic](#9-blocking-logic)
10. [Build & Run](#10-build--run)
11. [Output Explanation](#11-output-explanation)

---

## 1️⃣ What is DPI?
**Deep Packet Inspection (DPI)** is a method of network packet filtering that examines the data part (and possibly the header) of a packet as it passes an inspection point.

### 💡 Why DPI is useful
* **ISPs:** Bandwidth management and traffic shaping.
* **Enterprise:** Blocking unauthorized social media or high-risk sites.
* **Security:** Detecting malware patterns and intrusion attempts.
* **Parental Control:** Restricting access to specific domains.

### 🧠 System Logic
`Input (PCAP file)` ➡️ `DPI Engine` ➡️ `Output (Filtered PCAP)`
The engine performs:
* **Application Detection**
* **Rule Enforcement** (Blocking)
* **Statistical Reporting**

---

## 2️⃣ Networking Basics
### 🌐 Network Layers (Simplified)
| Layer | Protocols |
| :--- | :--- |
| **Application** | HTTP, TLS, DNS |
| **Transport** | TCP, UDP |
| **Network** | IP (IPv4/IPv6) |
| **Data Link** | Ethernet (MAC) |

### 📦 Packet Structure
Packets are encapsulated like Russian nesting dolls:
`Ethernet` ➡️ `IP` ➡️ `TCP` ➡️ `Payload (e.g., TLS Client Hello)`

### 🔑 The Five-Tuple
A connection "flow" is uniquely identified by:
1. Source IP
2. Destination IP
3. Source Port
4. Destination Port
5. Protocol

### 🔐 What is SNI?
When browsing an HTTPS site, the browser sends a **TLS Client Hello**. Even though the subsequent data is encrypted, the **Server Name Indication (SNI)**—the domain name (e.g., `www.youtube.com`)—is sent in **plaintext**. This is the "hook" DPI uses to identify encrypted apps.

---

## 3️⃣ Project Overview
The engine follows a linear pipeline:
`PCAP File` ➡️ `Parse` ➡️ `Classify` ➡️ `Apply Rules` ➡️ `Output PCAP`

### 🧩 Versions Included
* **Simple Version:** Sequential processing for easy debugging and learning.
* **Multi-threaded:** Optimized for high-throughput using worker pools.

---

## 4️⃣ File Structure
* `include/`: Header files (.h)
* `src/`: Implementation files (.cpp)

| Key File | Responsibility |
| :--- | :--- |
| `pcap_reader` | Handles file I/O and PCAP validation. |
| `packet_parser` | Extracts headers and handles byte-order conversion. |
| `sni_extractor` | Deep-dives into TLS payloads to find domains. |
| `types` | Definitions for Five-Tuple and Flow structures. |
| `dpi_mt.cpp` | The core multi-threaded engine logic. |

---

## 5️⃣ Packet Flow (Simple Version)
1.  **Read PCAP:** Open file and validate global headers.
2.  **Read Packet:** Pull raw bytes and packet metadata.
3.  **Parse Packet:** Extract MAC, IP, Ports, and Protocol.
4.  **Flow Creation:** Map the packet to a `FlowData` object using the 5-tuple.
5.  **Extract SNI:** If TLS is detected, parse the handshake for the domain.
6.  **Apply Rules:** Compare against blocked IPs, Apps, or Domains.
7.  **Forward/Drop:** Write allowed packets to the output file; discard blocked ones.
8.  **Stats:** Increment counters for the final report.

---

## 6️⃣ Multi-threaded Version
### ⚡ Architecture
`Reader` ➡️ `Load Balancer` ➡️ `Worker Threads` ➡️ `Output Writer`

### 🧠 Consistent Hashing
To ensure thread safety without heavy locking, we use **Consistent Hashing**. This ensures that all packets belonging to the **same flow** are always sent to the **same thread**, preventing race conditions during flow state updates.

---

## 7️⃣ Core Components
* **PCAP Reader:** Robust handling of the PCAP file format.
* **Packet Parser:** Converts Network Byte Order (Big Endian) to Host Byte Order.
* **SNI Extractor:** A state machine that identifies the TLS Handshake Type `0x01` (Client Hello).
* **Thread-Safe Queue:** Manages communication between the Reader and Workers.

---

## 8️⃣ SNI Extraction
### 🔐 The TLS Handshake
1.  **Client Hello** ⬅️ *DPI Inspects this (Plaintext SNI here)*
2.  **Server Hello**
3.  **Encrypted Data** ⬅️ *DPI cannot read this*

---

## 9️⃣ Blocking Logic
### 🛑 Types of Blocking
* **IP-Based:** Blocks specific destination addresses.
* **App-Based:** Blocks identified signatures (e.g., YouTube).
* **Domain-Based:** Blocks specific SNI strings (e.g., Facebook).

### 🔄 Flow-Based Decision
The first few packets of a handshake are allowed so the engine can see the "Client Hello." Once the app is identified and a "Block" rule is triggered, the entire **Flow** is marked as blocked, and all subsequent packets are dropped.

---

## 🔟 Build & Run
### 🔧 Compilation (Windows/GCC)
```powershell
g++ -std=c++17 -pthread -O2 -I include -o dpi_engine.exe src/dpi_mt.cpp src/pcap_reader.cpp src/packet_parser.cpp src/sni_extractor.cpp src/types.cpp
▶️ Basic Execution
PowerShell
.\dpi_engine.exe test_dpi.pcap output.pcap
🚫 Running with Blocking Rules
PowerShell
.\dpi_engine.exe test_dpi.pcap output.pcap --block-app YouTube
11️⃣ Output Explanation
The system generates a summary report upon completion:

Total Packets: Count of all packets analyzed.

Forwarded vs. Dropped: Efficiency of your blocking rules.

Application Breakdown: Visualization of traffic types (HTTPS, DNS, etc.).

Detected Domains: List of all unique SNIs extracted during the session.

🚀 Key Learnings
Protocol Analysis: Real-world parsing of Ethernet and IP headers.

Modern C++: Usage of std::thread, std::mutex, and std::map.

Security Insight: Understanding that "Encryption" does not mean "Invisible."

🏁 Conclusion
This engine serves as a functional prototype of industry-standard systems used by ISPs and security vendors worldwide. It highlights the delicate balance between high-speed performance and deep data analysis.