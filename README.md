# Fightclub

**Current stable, distributed versions:**

 - Department 0.5
 - Teleprinter 0.8
 - Dashboard 0.1


## About Fightclub
Fightclub aims to become an enhanced clock system for the [International Young Physicist's Tournament (IYPT)](http://iypt.org).
Currently, heavy development is in progress, so design, workflow and requirements may change between two versions and result in incompatibilities.


Fightclub currently consists of the following components:

 - *Fightclub Department*, an application for timing a physics fight.
 - *Fightclub Teleprinter*, a remote clock window for use on remote machines inside the local network.
 - *Fightclub Dashboard*, displaying the state of multiple clients


Following components are considerd as “useful” and may be added soon:

 - *Fightclub Disperser*, a tool for duplicating broadcast signals emitted by Fightclub Department
 - *Fightclub Nano*, a clock window with an own timing engine, but not as powerful as *Fightclub Department*


## Fightclub Department
Fightclub Department uses plain text files for loading data. They can be created and edited using a common text editor.  
Please review the [requirements on these data files](docs/fightclub-file-requirements.pdf).

### Screenshots
Fightclub Department is equipped with a clear, intuitive interface for managing the physics fight.  
![Fightclub Department control interface](docs/screenshot-department.png)

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
Fightclub uses:

 - the Qt framework,
 - the “Breeze” icon theme and
 - some components of the [QIYPTClock](https://github.com/drogenlied/QIYPTClock)

The respective copyright holders are listed in the headers of the source files and in the application's “About” dialog.
  
