#### Current stable, distributed versions:
 - Client 0.2
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


### Requirements on data files
Fightclub Client uses plain text files for loading data. They can be created and edited using a common text editor; however,
those files require use of the respective file extension listed below and have to be encoded using UTF-8 (without Byte Order Mark).

To avoid loading wrong files – and the resulting malfunction – a file header, consisting of some XML code, may be added
to the first lines of a data file:

    <FightclubExchangeFile>stages</FightclubExchangeFile>
    <ExchangeProtocolVersion>1</ExchangeProtocolVersion>
    <ExchangeFileTitle>Stages for some tournament</ExchangeFileTitle>
    <ExchangeFileContentDescr>This is only an example with metasyntactical values</ExchangeFileContentDescr>
    
*Note:*
 - The tags are *case sensitive*: `<FightclubExchangeFile>` gets recognized, while `<FIGHTCLUBexchangefile>` does not.
 - Line breaks inside a tag are *not* allowed; the whole tag must be opened and closed on the same line.
 - The “proper” values for `<FightclubExchangeFile>` and `<ExchangeProtocolVersion>` depend on the exchange file and are listed below

As long as not being stated otherwise, `\t` represents a tabstopp. It is *not* possible to use spaces for indentation.


#### Files specifying stages

File extension | `.fcstages`
`<FightclubExchangeFile>` | `stages`
`<ExchangeProtocolVersion>` | `1`




#### Files specifying phases

File extension | `.fcphases`
`<FightclubExchangeFile>` | `phases`
`<ExchangeProtocolVersion>` | `1`


##### General Syntax
    duration [sec] \t overtime [sec] \t title [string] \t performances [string] \t options [string]
    
    
 - `duration`: the maximum duration of the phase in seconds
 - `overtime`: the maximum allowed overtime in seconds
 - `title`: the title of the phase as displayed
 - `performances`: the roles performing during this phase
 - `options`: futher configuration of the phase
    

You may omit

 - the `options` column or
 - the `performances` column together with the `options` column, but *not* the `performances` column alone.

Lines consisting of less than three tabular-separated columns are treated as comments and ignored.

    
##### Allowed values for `performances`
 - `rep`: the Reporter is performing during the current phase
 - `opp`: the Opponent is performing during the current phase
 - `rev`: the Reviewer is performing during the curr
 - `nll`: none of the three listed above are performing in the current phase

You have to specify *at least one* of these values, including `nll`. Values can be combined, e.g. one may use `repopp`
for the discussion phase.
    
##### Available `options`
 - `a`: Autoadvance to the next phase, if maximum duration *and* allowed overtime have elapsed
 - `c`: Carry the *whole elapsed* time to the next phase
 - `o`: Only carry the elapsed overtime to the next phase
 - `r`: Show the roomclock. Use this option only for phases with no maximum duration.
 - `n`: Do not use any options and behave like a “normal” phase.

Again, you have to specify *at least one* of these values, including `n`.

##### Examples
The following lines represent only single lines of a `phases` file. As above, `\t` represents a tabstopp:

    720000  \t 0  \t Presentation of the report                        \t rep     \t n
    240000  \t 0  \t The Opponent takes the floor                      \t opp     \t c
    840000  \t 0  \t Discussion between the Reporter and the Opponent  \t repopp  \t n
    120000  \t 0  \t Preparation of the Reviewer                       \t rev     \t n
    1000    \t 0  \t Pause                                             \t nll     \t r

For further examples see the phases files inside the `sample-files` directory.








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
  
