# MP2P

[![TeamCity Build Status](https://img.shields.io/teamcity/http/42portal.com/teamcity/s/Mp2p_Build.svg?style=flat?label=TeamCity)](https://42portal.com/teamcity/viewType.html?buildTypeId=Mp2p_Build)
[![Join the chat at https://gitter.im/Dubrzr/MP2P](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Dubrzr/MP2P?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

File transfer application using a custom protocol.

# Details

This project is divided into three different applications:

* Storage
* Master
* Client

They are all based on `libmp2p`.

The library implements functions for the MP2P protocol.
It is using **Boost.Asio** for the networking part.

All the apps are using the library for network communications and file
manipulations.

The project uses **Couchbase** as a Master-Master replication database system.

## How to?

### Config files

We provide some **\*.conf.example** files in the config folder,
you can configure them to set up your own settings.

## Requirements

* C++ >= 14
* Boost >= 1.58
* OpenSSL >= 1.0
* libcouchbase == 2.4.8

## Details

* The Master application is there to make a connection between all the clients
and the storage. It will be making continous database updates/checks in order
to ensure the integrity of the transfers.

* The Storage application is there to listen for packets and send/receive
files.

## Tests

We are using the [Catch](https://github.com/philsquared/Catch) unit test
framework for our tests.

`make check` will launch the unit tests.
