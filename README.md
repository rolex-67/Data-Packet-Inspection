# 🚀 DPI Engine - Deep Packet Inspection System (Student Version)

This project explains how a **Deep Packet Inspection (DPI)** system works — from basic networking concepts to a full implementation in **C++**.

---

## 🎯 What You’ll Learn

After going through this project, you will be able to:

- Understand how **network packets are structured**
- Explain how **DPI works (even with HTTPS)**
- Describe how **packets flow through a system**
- Confidently explain this project in **technical interviews**

---

## 📚 Table of Contents

1. [What is DPI?](#1-what-is-dpi)
2. [Networking Basics](#2-networking-basics)
3. [Project Overview](#3-project-overview)
4. [File Structure](#4-file-structure)
5. [Packet Flow (Simple Version)](#5-packet-flow-simple-version)
6. [Packet Flow (Multi-threaded Version)](#6-packet-flow-multi-threaded-version)
7. [Core Components](#7-core-components)
8. [SNI Extraction](#8-sni-extraction)
9. [Blocking Logic](#9-blocking-logic)
10. [Build & Run](#10-build--run)
11. [Output Explanation](#11-output-explanation)

---

## 1️⃣ What is DPI?

**Deep Packet Inspection (DPI)** means analyzing not just packet headers, but also the **actual content inside packets**.

### 💡 Why DPI is Useful

- 🌐 ISPs → Control bandwidth usage  
- 🏢 Companies → Block social media  
- 🔐 Security → Detect threats  
- 👨‍👩‍👧 Parental Control → Restrict websites  

---

### 🧠 What This Project Does
Input (PCAP) → DPI Engine → Output (Filtered PCAP)
↓
- Detects applications
- Applies blocking rules
- Generates statistics

Input (PCAP) → DPI Engine → Output (Filtered PCAP)
↓
- Detects applications
- Applies blocking rules
- Generates statistics


---

## 2️⃣ Networking Basics

### 🌐 Network Layers (Simplified)
Application → HTTP, TLS
Transport → TCP / UDP
Network → IP
Data Link → MAC


---

### 📦 Packet Structure

Each packet is layered like this:
Ethernet → IP → TCP → Payload


Example Payload:
- TLS Client Hello (contains domain name)

---

### 🔑 Five-Tuple (VERY IMPORTANT)

A connection is identified by:

- Source IP  
- Destination IP  
- Source Port  
- Destination Port  
- Protocol  

👉 All packets with the same 5-tuple belong to the **same flow**

---

### 🔐 What is SNI?

When you visit a website:

- Browser sends **TLS Client Hello**
- Domain name is sent in plaintext (SNI)

Example: www.youtube.com


👉 This allows DPI to detect apps even in HTTPS

---

## 3️⃣ Project Overview
PCAP File → Parse → Classify → Apply Rules → Output PCAP

PCAP File → Parse → Classify → Apply Rules → Output PCAP
packet_analyzer/
├── include/ # Header files
├── src/ # Implementation


### Key Files

- `pcap_reader` → Reads packets  
- `packet_parser` → Extracts headers  
- `sni_extractor` → Extracts domain  
- `types` → Data structures  
- `dpi_mt.cpp` → Main engine  

---

## 5️⃣ Packet Flow (Simple Version)

### Step 1: Read PCAP
- Open file  
- Validate header  
- Start reading packets  

### Step 2: Read Packet
- Header (metadata)  
- Data (actual bytes)  

### Step 3: Parse Packet
Extract:
- MAC address  
- IP address  
- Ports  
- Protocol  

### Step 4: Flow Creation

```cpp
Flow = map<FiveTuple, FlowData>
Step 5: Extract SNI

Detect TLS Client Hello

Extract domain

Step 6: Apply Rules

Blocked IP

Blocked App

Blocked Domain

Step 7: Forward or Drop

✅ Allowed → Write to output

❌ Blocked → Discard

Step 8: Generate Stats

Count packets

Count apps

Print report

6️⃣ Packet Flow (Multi-threaded Version)
⚡ Architecture
Reader → Load Balancer → Workers → Output

🧠 Key Idea: Consistent Hashing

👉 Same flow → Same thread

This ensures:

Correct tracking

No race conditions

🔄 Flow

Reader reads packet

Sends to Load Balancer

LB sends to worker thread

Worker processes packet

Output thread writes result

🔒 Thread Safety

Mutex

Condition variables

Thread-safe queue

7️⃣ Core Components
📂 PCAP Reader

Reads file format

Extracts packets

🔍 Packet Parser

Parses Ethernet, IP, TCP

Handles byte order

🌐 SNI Extractor

Detects TLS handshake

Extracts domain

🧱 Types

FiveTuple

Flow

AppType

8️⃣ SNI Extraction
🔐 TLS Handshake
Client → Client Hello (SNI visible)
Server → Server Hello
Data   → Encrypted

🔑 Key Insight

👉 Only Client Hello is readable
👉 After that → everything is encrypted



🧠 Extracted Data
SNI = "www.youtube.com"
9️⃣ Blocking Logic
🛑 Types of Blocking
Type	Example
IP	192.168.x.x
App	YouTube
Domain	facebook
🔄 Flow-Based Blocking
Initial packets → allowed  
Client Hello → detect app  
Mark flow as blocked  
All future packets → dropped  
10️⃣ Build & Run
🔧 Compile (Windows PowerShell)
g++ -std=c++17 -pthread -O2 -I include -o dpi_engine.exe src/dpi_mt.cpp src/pcap_reader.cpp src/packet_parser.cpp src/sni_extractor.cpp src/types.cpp
▶️ Run
.\dpi_engine.exe test_dpi.pcap output.pcap
🚫 Run with Blocking
.\dpi_engine.exe test_dpi.pcap output.pcap --block-app YouTube
11️⃣ Output Explanation
📊 Output Includes

Total packets processed

Forwarded vs Dropped

Thread distribution

Application breakdown

Detected domains

🧠 Meaning
Field	Meaning
Forwarded	Allowed packets
Dropped	Blocked packets
Apps	Traffic classification
Domains	Extracted SNI
🚀 Key Learnings

This project demonstrates:

Network protocol understanding

Packet parsing

TLS handshake analysis

Flow-based processing

Multi-threading

Producer-consumer design

💡 Final Insight

Even though HTTPS is encrypted,
the domain name is still visible in the TLS handshake.

👉 This is the core idea behind DPI systems

🏁 Conclusion

This project is a mini real-world DPI engine that shows how:

Networks are analyzed

Applications are identified

Traffic is controlled