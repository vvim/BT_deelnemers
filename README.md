BT_deelnemers
=========

Dit is een eerste widget om het software-gedeelte voor het Buurtijd-project voor te bereiden.

Er is gekozen voor Qt en C++ omdat ik me daar gewoon het meest comfortabel in voel. Om geen grote verschillen te maken met mijn andere projecten, kies ik voorlopig nog steeds voor Qt4.

Dit project is gebaseerd op de code van qt-google-maps. Zie http://stackoverflow.com/a/38720029/707700 voor mijn visie over het implementeren van de Google Maps API in je Qt/C++-project.

installatie-tips
-------------

Zoals hierboven vermeld, zie http://stackoverflow.com/a/38720029/707700 voor het gedeelte rond de Google Maps API

# Debian

## 1] Basic QT-installation and to use a MySQL connection:

    sudo apt-get install qt4-default
    sudo apt-get install qtcreator
    sudo apt-get install build-essential g++
    sudo apt-get install git git-gui
    sudo apt-get install libqt4-sql-mysql
    sudo apt-get install libmysqlclient-dev
    sudo ln -s /usr/lib/x86_64-linux-gnu/libmysqlclient.so /usr/lib/libmysqlclient.so

## 2] In case you want a local MySQL database for testing:
    sudo apt-get install mysql-server
    sudo apt-get install phpmyadmin
    sudo ln -s /usr/share/phpmyadmin/ phpmyadmin

Now you can surf to http://localhost/phpmyadmin .

Create a new user `testuser` with  `HiDrNick!` as password and all privileges to database `buurtijd_test`.

The code uses a QSettings INI-file to connect to the database. Create a file called `settings.ini` and put it in the build directory of your Qt Project (probably something like `../build-BT_deelnemers-Desktop-Debug/`).

This file should contain the information to connect to the database. Mos basic example:

    [db]
    host=localhost
    username=testuser
    password=HiDrNick!
    databasename=buurtijd_test



## 3] For installing QJSON:

see also the README at https://github.com/vvim/routetool/blob/c7bfc8f4c556bd202083cc0289a354a0ecd51e5a/README.md

 * install `cmake`


        sudo apt-get install cmake

 * `/codedir$` represents the directory where you want to put the `qjson`


        /codedir$ git clone git://github.com/flavio/qjson.git
        /codedir$ cd qjson
        /codedir/qjson$ mkdir build
        /codedir/qjson$ cd build
        /codedir/qjson/build$ cmake ..
        /codedir/qjson/build$ make
        /codedir/qjson/build$ sudo make install
        /codedir/qjson/build$ sudo cp lib/libqjson.so.0 /usr/lib/


Any problem? Please contact me directly.



<<< on MarkDown markup, see https://help.github.com/articles/writing-on-github and http://daringfireball.net/linked/2014/01/08/markdown-extension >>>

<<< I like using http://dillinger.io/ as an online editor >>>
