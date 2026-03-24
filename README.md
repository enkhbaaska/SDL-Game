# SDL-Game

A simple turn-based battle game built in C++ using SDL3. The project was created to practise game programming, rendering, input handling, and structuring a small interactive application in modern C++.

## Features

- Character-based battle gameplay
- SDL3 window creation and rendering
- Image loading with SDL3_image
- Font rendering with SDL3_ttf
- Menu / interface elements
- Basic game logic for combat and interaction

## Tech Stack

- C++
- SDL3
- SDL3_image
- SDL3_ttf
- Makefile build setup

## Project Structure

- `src/main.cpp` – main game logic and rendering
- `res/` – game assets such as images
- `font/` – font files
- `Makefile` – build instructions

## Why I Built It

I built this project to improve my C++ and game development skills by creating a small playable application from scratch. It helped me practise working with graphics libraries, managing assets, handling user input, and organising a game project.

## How to Build

Make sure SDL3, SDL3_image, and SDL3_ttf are installed and available.

```bash
make
g++ main.cpp