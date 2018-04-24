#### Current stable, distributed versions:
 - Client 0.3
 - Remote Clock Window 0.6


## About Fightclub
Fightclub aims to become an enhanced clock system for the [International Young Physicist's Tournament (IYPT)](http://iypt.org).
Currently, heavy development is in progress, so design, workflow and requirements may change between two versions and result in incompatibilities.


Fightclub currently consists of the following components:

 - *Fightclub Client*, an application for timing a physics fight.
 - *Fightclub Remote Clock Window*, a remote clock window for use on remote machines inside the local network.


Following components are planned to be added:

 - A remote clock window displaying the state of multiple clients



## Fightclub Client


### Screenshots
Fightclub Client is equipped with a clear, intuitive interface for managing the physics fight.  
![Fightclub Client control interface](docs/screenshot-client.png)

During a physics fight, the embedded clock window displays elapsed time as well as the well-known “progress pie-chart.”  
![The clock window during a physics fight](docs/screenshot-clockwindow.png)

For other points of order, the current time may be displayed.  
![The clock window during a break](docs/screenshot-roomclock.png)












## Legal Notes
### License
Fightclub is licensed under version 3 of the GNU General Public License.  
Please review the following information to ensure the GNU General Public License version 3.0 requirements will be met:
[http://www.gnu.org/copyleft/gpl.html](http://www.gnu.org/copyleft/gpl.html)



### Third party libraries
Fightclub Client uses:

 - the Qt framework,
 - the “Breeze” icon theme and
 - some components of the [QIYPTClock](https://github.com/drogenlied/QIYPTClock)

The respective copyright holders are listed in the headers of the source files and in the application's “About” dialog.
  
