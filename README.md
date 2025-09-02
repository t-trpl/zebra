# zebra

A portable C++-17 command line utility that stripes files into pieces, and assemble them back together. 

---

## Modes

- **Stripe** - Stripe files into pieces
- **Assemble** - Assemble pieces from a directory
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
   chmod +x zebra/abuild.sh &&
   ./abuild.sh
   ```
3. Binary location ./build/zebra
