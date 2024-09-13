# Assignment 4: Map-Reduce RPC
Due March 18, 2024 11:59 PM
Instructions
Complete the missing functions in the given program to implement Map Reduce using RPC. 

The coordinator behaves as an RPC server and the worker nodes as RPC clients. Evaluation will be done based on the test-cases given. To run the test case, use the following command:
./test-mr-many.sh <num-of-trials>

The tests would run multiple times based on the <num-of-trials> parameter.

Set-up Instructions
Disable Go Modules and set your GOPATH by using the following commands

export GO111MODULE=off
export GOPATH="<assignment-folder-path>"

If you have any doubts in the assignment, feel free to reach out to me.

Submit a brief write-up explaining your implementation as a .md/ .txt/ .pdf file.

Note:
Sequential Implementation of the problem can be found at src/mrsequential.go. You would need to use a similar approach to import the wc.so file and use the functions implemented in them.  
