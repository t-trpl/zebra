# zebra

A portable C++17 command-line utility to stripe and reassemble files easily. Similar to GNU split.

---

## Modes

- **Stripe** - Stripe files into pieces
- **Assemble** - Assemble stripes (or matching files) from a directory
- **Assemble Multi** - Assemble all files provided as arguments

---

## Installation

### Requirements
- gcc
- cmake
---
### How to build

1. Clone the repository:
   ```bash
   git clone https://github.com/t-trpl/zebra
   ```
2. Run the build script
   ```bash
   cd zebra &&
   chmod +x abuild.sh &&
   ./abuild.sh
   ```
3. Binary location ./build/zebra
