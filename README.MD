# Daecon - Deamon connect

## Overview

Daecon is a simple logging daemon for Linux servers. It consists of a client and server component, allowing you to send log messages from clients to a centralized server for storage and analysis.

![Screenshot](https://nasiadka.pl/projects/Daecon/Daecon.png)

## BE AWARE

Code is nasty, dangerous and doesn't do most of thing I want it to do (for now)

## Running the program

### To run as a server (daemon):

`daecon -d`
or
`daecon -d p (port)`

### to run as a client:

`daecon <message_type> <sender_info> <message_content> `
or
`daecon <message_type> <sender_info> <message_content>`

### to see all messages since daemon start:

`daecon`
or
`daecon -p port`

## How to build and install

### Build

```bash
git clone https://github.com/NasiadkaMaciej/Daecon
cd Daecon
./build.sh
```

### Install

```bash
cd Daecon
sudo ./install.sh
```
