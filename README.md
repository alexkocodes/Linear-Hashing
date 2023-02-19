# Linear Hashing Project - Poll Monitor

## Description 
This is a C program that uses linear hashing to monitor votes across different people, and it provides functionalities such as quick look up, registering vote statutes, bulk vote, check the number of votes, and organize votes in a zip-based manner. 


## How To Run 
1. run **make**
2. run **./mvote -f *registeredvotersfile* -b *bucketentries***

(flag -f indicates the text-file named registeredvoters, containing an initial set of qualified poll participants, and flag -b designates the maximum number of keys each hash-table bucket can accommodate.)

Note: You can customize the command by specifying *registeredvotersfile* and *bucketentries*. *registeredvotersfile* is the path to the csv file that contains all the personal information of each voter, and *bucketentries* is the maximum number of nodes each bucket can contain.  

