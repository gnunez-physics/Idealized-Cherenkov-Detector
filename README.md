# Idealized Water Cherenkov Detector Simulation

[![Geant4](https://img.shields.io/badge/Geant4-Toolkit-blue.svg)](https://geant4.web.cern.ch/)
[![C++](https://img.shields.io/badge/C++-Standard-orange.svg)](https://en.cppreference.com/)
[![ROOT](https://img.shields.io/badge/CERN-ROOT-informational.svg)](https://root.cern/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A Monte Carlo simulation project developed using **Geant4** to model an idealized Water Cherenkov Detector. This repository is part of applied research in high-energy physics instrumentation, bridging theoretical particle propagation in dielectric media with realistic sensor response analysis.

---

## 📌 Project Overview

When relativistic charged particles travel through a dielectric medium (such as water) at a speed exceeding the phase velocity of light in that medium, they emit **Cherenkov radiation**. This project simulates this phenomenon to evaluate optical photon propagation, energy deposition, and detector geometrical acceptance.

The simulation models:
* Particle interactions and secondary track generation.
* Optical photon production and propagation within water.
* Hit registration, energy deposition (`EnergyHits`), and timing analysis.
* Sensor displacement studies to evaluate efficiency variations based on offset configurations.

---

## 📂 Repository Structure

```text
├── include/           # Header files (.hh) for detector geometry, physics list, and sensitive detectors
├── src/               # Implementation files (.cc) for simulation physics and event processing
├── macros/            # Geant4 macro control files (.mac) for beam configuration and runs
├── data/              # Output ROOT files (.root) organized by displacement parameters
├── build/             # Compilation directory and build configuration
├── CMakeLists.txt     # CMake configuration file for compiling the project
└── muon.cc            # Main C++ executable source file
