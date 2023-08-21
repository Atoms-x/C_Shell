<p align="center">
  <img alt="C_Shell" src="https://github.com/SgtR0ck/C_Shell/assets/8111664/e9c828ef-2ae7-435f-9cf0-be856ef51830"
</p>
<h1 align="center"> 
  C Shell 
</h1>

<p align="center">
  <a href="https://github.com/SgtR0ck/City_Simulator">
    <img src="https://img.shields.io/badge/version-1.0.0-green.svg?style=plastic">
  </a>
  <img src="https://img.shields.io/badge/language-C-%2300599C.svg?style=plastic&logo=C">
  <img src="https://img.shields.io/badge/code%20style-ANSI-orange.svg?style=plastic">
  <a href="https://github.com/SgtR0ck/City_Simulator/blob/main/LICENSE.md">
    <img src="https://img.shields.io/badge/license-MIT-green.svg?style=plastic">
  </a>
</p>

## Table of Contents
  - [Introduction](#introduction)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Formatting](#formatting)
    - [Running Batch Mode](#running-batch-mode)
    - [Running Interactive Mode](#running-interactive-mode)
  - [Additional Features](#additional-features)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)


## Introduction

This is a C based program, a command-line interpreter (shell), which takes commands typed into the CLI and passes them to the OS to perform.

## Getting Started

### Prerequisites

Be sure to have a C compiler installed. Additionally, an IDE is recommended.
Try following this guide for [Visual Studio Code](https://code.visualstudio.com/docs/languages/cpp)

![image](https://github.com/SgtR0ck/City_Simulator/assets/8111664/73855d8b-0999-4c9b-8434-73d49ee4a338)

### Formatting

While not necessary to run the program, a batch file can be used with the program in batch mode. Commands should be given on each line, with multiple commands separated by a ';'. The formatting should look like this:

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/57cdbc7b-f9e1-4c84-ba12-381da86191da)

### Running Batch Mode

Once you've executed the program, your CLI should present a prompt 

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/22bb578b-2dc9-4c7f-b501-6b068020fc1a)

Type "newshell" followed by a space and the name of the batch file (for batch mode)

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/e53964e0-ec48-44c0-a85b-d62cf54d597a)

Wait for the shell to load

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/28b9ccda-2988-4fd2-94d6-96f4fd74f7ba)

Then watch as your commands are executed, line-by-line

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/07ba3eb4-ce2e-476f-97bf-e0ad392a8f04)

### Running Interactive Mode

Once you've executed the program, your CLI should present a prompt 

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/22bb578b-2dc9-4c7f-b501-6b068020fc1a)

Type "newshell" to enter into interactive mode

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/5267297a-5a0c-4bea-9c6b-f2fd7986eacb)

Wait for the shell to load

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/28b9ccda-2988-4fd2-94d6-96f4fd74f7ba)

You will then be presented with a CLI terminal to enter commands. Your PWD (print working directory) will be displayed before and after each command.

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/55afb803-1ba9-4dc6-8c7e-04ff6953130a)

Give a command

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/8d271483-cdad-4dde-bdb0-7ef7b28ea1f2)

and watch the outcome

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/db6704e7-6c54-488f-90cc-7b071b4a5a34)

## Additional Features

* **exit** - An exit command can be given at any point, either by itself or as part of a chain of commands. No matter where it is placed, it will always be the last command executed

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/94ac9215-86f1-4a66-af5e-e096325fce37)

* **piping** - Piping is supported using the '|' key to pipe outputs to new commands. However, only three commands (two pipes) are supported

![image](https://github.com/SgtR0ck/C_Shell/assets/8111664/a4ebd695-a34c-4a8e-9b30-1d2fc562bbfb)

## Authors

* **Chet Lockwood** - [Atoms-x](https://github.com/Atoms-x)

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/SgtR0ck/C_Shell/blob/main/LICENSE.md) file for details

## Acknowledgments

* Thank you to the many guides across the internet on C++
