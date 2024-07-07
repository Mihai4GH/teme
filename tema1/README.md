*** The main functionality ***

**Train Structure** - contains the UPDATE, SEARCH, QUERRY and EXECUTE operations that are to be applied on the train and the circular list that the train structure is based on. Also is contains a function that procceses the commands that are to be applied on the train.

*Search* : iterate trough every car. If a match is detected eter a loop to check for the whole word. If its found conductor is moved acordingly. Otherwise breaks out of the loop and continue the search at the very next position from witch the match of the first character occured. Continues untill conductor's car (end of train for SEARCH_LEFT/RIGHT) is meet.

**Queue Structure** - The queue has to 3 fields: 2 poiters to the first and last nodes, and a flag (QSwitch) that determines in which way are operations executed on the queue. By doing that it doesnt waste time by switching the whole queue, which in some cases might be time consuming.

*Enqueue* - if QSwitch is set to 1, enques at the end of queue; if it's set to -1, enqueus at the start of queue
*Execute* - if QSwitch is set to 1, executes from the start of queue; if QSwitch is set to -1 from the end

**Main** - The train and queue are initialized. Reading from files is performed with scanf function, and by considering everything a string. If there are arguments needed they are read. 