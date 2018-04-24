---
---

# Requirements on data files
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


## Files specifying stages

File extension | `.fcstages`
`<FightclubExchangeFile>` | `stages`
`<ExchangeProtocolVersion>` | `1`

### General Syntax
There are two kinds of stages: stages of a physics fight and so-called “roomclock stages”, such as adresses of welcome, lunch breaks, …

Depending on whether Reporter, Opponent and Reviewer are present, one of the following lines indicates a physics fight's stage:

    stage label \t problem \t reporter id \t opponent id \t reviewer id
    stage label \t problem \t reporter id \t opponent id
    stage label \t problem \t reporter id

 - `stage label`: should be used to indicate fight, room and stage nr (e.g. `PF 3/D1` for stage 1 in room D of physics fight 3)
 - `problem`: either the problem's number or `-1` (when specifying `-1`, the problem may be selected from a list)
 - `reporter id`, `opponent id`, `reviewer id`: either the *team id* of the reporting, opposing or reviewing team or the *personal id* of the Reporter, Opponent or Reviewer

The following line indicates a roomclock stage:

    stage description

### Examples
    PF 3/D1  \t -1  \t  ger  \t cze  \t uk
    Lunch break

For further examples see the stages files inside the `sample-files` directory.



## Files specifying phases

File extension | `.fcphases`
`<FightclubExchangeFile>` | `phases`
`<ExchangeProtocolVersion>` | `1`


### General Syntax
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

    
### Allowed values for `performances`
 - `rep`: the Reporter is performing during the current phase
 - `opp`: the Opponent is performing during the current phase
 - `rev`: the Reviewer is performing during the curr
 - `nll`: none of the three listed above are performing in the current phase

You have to specify *at least one* of these values, including `nll`. Values can be combined, e.g. one may use `repopp`
for the discussion phase.
    
### Available `options`
 - `a`: Autoadvance to the next phase, if maximum duration *and* allowed overtime have elapsed
 - `c`: Carry the *whole elapsed* time to the next phase
 - `o`: Only carry the elapsed overtime to the next phase
 - `r`: Show the roomclock. Use this option only for phases with no maximum duration.
 - `n`: Do not use any options and behave like a “normal” phase.

Again, you have to specify *at least one* of these values, including `n`.

### Examples
The following lines represent only single lines of a `phases` file. As above, `\t` represents a tabstopp:

    720000  \t 0  \t Presentation of the report                        \t rep     \t n
    240000  \t 0  \t The Opponent takes the floor                      \t opp     \t c
    840000  \t 0  \t Discussion between the Reporter and the Opponent  \t repopp  \t n
    120000  \t 0  \t Preparation of the Reviewer                       \t rev     \t n
    1000    \t 0  \t Pause                                             \t nll     \t r

For further examples see the phases files inside the `sample-files` directory.
