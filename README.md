# Computer Network Assignments

This repository contains two assignments completed for a university-level **Computer Networks** course.  
All programs are implemented in **C++**, focusing on networking principles and protocol simulation.

---

## 🧑‍💻 Assignments Overview

### HW1: Cyclic Redundancy Check (CRC)
- Implements **error detection** using CRC (Cyclic Redundancy Check).
- Two main programs:
  - `crc_encoder`: Encodes an input file using a given generator and dataword size.
  - `crc_decoder`: Decodes and validates the encoded file, detecting errors and recovering the original data.
- Key Features:
  - Modulo-2 division to generate CRC bits
  - Bit-level I/O operations
  - Zero-padding and metadata handling
  - Frame error detection and result logging
- Example usage:
  ```bash
  ./crc_encoder input_file output_file generator dataword_size
  ./crc_decoder input_file output_file result_file generator dataword_size
  ```

  ### 🔄 Example Flow (HW1: CRC)

1. **Encode original file** → `crc_encoder`
2. **Simulate transmission errors** → `linksim`
3. **Decode and validate received file** → `crc_decoder`

---

### 🌐 HW2: Routing Protocol Simulation

Simulates routing table construction and updates using:

- **Link State algorithm (Dijkstra)**
- **Distance Vector algorithm**

#### 🔧 Programs
- `linkstate`: Implements Link State routing logic.
- `distvec`: Implements Distance Vector routing logic.

#### 📥 Inputs
- `topology.txt`: Initial network topology (nodes and link costs)
- `messages.txt`: Messages to route between nodes
- `changes.txt`: Changes in the network topology over time

#### 📤 Output
- Updated routing tables and message paths at each simulation step
- Output files:
  - `output_ls.txt` (Link State)
  - `output_dv.txt` (Distance Vector)

#### ▶️ Example Usage
```bash
./linkstate topology.txt messages.txt changes.txt
./distvec topology.txt messages.txt changes.txt
```

### 🛠 Technologies Used

- **Language**: C++
- **Platform**: Unix-based systems (Linux/macOS)
- **Concepts Covered**:
  - CRC encoding/decoding and error detection
  - Link State and Distance Vector routing algorithms
  - File parsing and structured input handling
  - Simulation logic and dynamic routing table updates