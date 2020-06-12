# Fault-Tolerance

## First Model 

**Overview**

The purpose of our application is to take in a unique identifier every 1 second and invoke each of the 6 methods with this identifier, until a true value is returned. The application uses retry logic (exponential back off) to solve the problem. The program runs across multiple threads which are synchronized properly to avoid any errors. It consists of a main thread that splits into 6 extra threads where each thread runs a particular method. Each sub thread is then split into 10 more threads to run for different identifiers at the same time. Along with that a few separate threads are also created to control the parameters of methods according to the program need. Below are the list of functions and classes that help run the application effectively. 

 ![](Images/model1%20framework.png)

### Functions 

**main function** 

The main function begins with calling below listed functions on new threads like calling 6 threads for function retryLimitingHandler , 6 threads for particularMethodExecutor where each thread works for a particular method from 1 to 6. Next it inputs data from files -> DegradedMode.txt and InputFile.txt. Then it starts adding one identifier after 1 sec to global vector variable identifierStorage. After finishing with inputs, it begins to join all the new threads which were created to the main thread. Then it starts writing the desired statistics in a csv file which include identifier name, identifier completion time, number of attempts, time at which each attempt is called and total time taken for that attempt. This is done for all methods and identifier and stored in the csv file for further plotting. 

**retryLimitingHandler function**

The aim of this function is to set retryLimit to 0 after every 1 second. This helps us in maintaining the desired 2 true values per second property of the normal mode. It takes in index as an argument to know which methods retryLimit it has to work on. It works on a separate thread from rest of the program. 

**setDegradationTime function** 

This function is used to set the degradation time for a particular method according to the need of the application. It sets this time randomly in the run time of program and it too works on a separate thread from the rest of the program. 

**stringToInt function**

This method is used to convert a string of numbers into integers. 

**particularMethodExecutor function** 

This function contains ten threads which could be activated to call randomSeedSetUp as soon as the application receives identifier input. It consists of two arrays named availableThread and threadClosure which help in initiating and joining these ten threads to the current function thread. Thus, this function is used to run 10 identifiers at a time for a method to speed up the performance in case of a backlog by degradation. It implements a particular method on its thread based on the index provided to it by the main function. 

**randomSeedSetUp function** 

This method is used to generate a specific seed for each thread before going to retryLogicImplementation function to prevent different threads from getting the same values from rand () and hence leading to the desired randomness. 

**retryLogicImplementation function**

This function applies the exponential back off retry logic for a particular identifier for a particular method. This is where we use the global metrics vector of timeStorage class to get the useful metric time values of the program that could later be used for statistical analysis. 

### Classes 

**Method1, Method2, Method3, Method4, Method5, Method6**

All these classes are similar whose main motive is to call the modeGuesser method(public), which on taking the identifier returns either true or false. Internally it has two private methods which are called normalMode and degradedMode and modeGuesser calls them randomly using the rand () function. Other desired properties which are needed to be satisfied by these classes are taken in care by an instance of MethodParameters. 

**TimeStorage**

This class is basically used to store the completion time of identifier, starting and ending time for each attempt for an identifier for all methods. It uses chrono library to store time in a format that is easily convertible to seconds later on. This class also stores the number of attempts it took for a particular identifier to successfully compete the method. 

**MethodParameters**

This class consist of attributes in form of 3 arrays of length 6 storing state, mode and retryLimit for each method. This class aim is to change these parameters of Methods according to the requirement of the program. Thus, it has its required getters and setters. Also, it contains a mutex which helps in synchronized execution of class across multiple threads. 

 

## Second Model 

**Overview**

The purpose of our application is to take in a unique identifier every 1 second and invoke each of the 6 methods with this identifier, until a true value is returned. The application uses retry logic (exponential back off) to solve the problem.  The program though runs across multiple threads but they are considerably limited as compared to first model. The main architecture here involves just division of main thread (specifically thread running the identifierDistributor function) into 10 extra different threads. Each thread implements a unique identifier but here it implements all the methods across the single thread in a sequential manner instead of having separate threads for each method. This helps in extending our application to dependent methods (example method2 can be only implemented after method1 for an identifier) and limiting our thread pool. 

 ![](Images/model2%20framework.png)

**Assumption** 
Dependency of methods are in order 1->2->3->4->5->6.

### Functions 

**main function** 

The main functions main purpose is to take inputs from files like DegradedMode.txt and InputFile.txt. After that it calls the identifierDistributor function across a new thread and then begins to provide a unique identifier every second via a global vector. Besides that, after implementation of retry logic it is used to store the necessary statistics in csv as done in first model. 

 **identifierDistributor function**

This function is called by the main function along a different thread. This function is basically used to create and maintain 10 threads with each thread acts all the methods for a particular identifier. Thus, it contains arrays like threadClosure and availableThread to maintain the thread pool. Besides this at the start it also calls retryLimitingHandler function and setDegradationTime function along different threads like the main function did in the First Model to maintain the desired properties of methods. 

**allMethodExecutor function**

This function calls all the methods in a sequential order and hence ends up acting like an extension to the problem statement where we can also work with methods which are dependent on each other. Thus, it ends up calling retryLogicImplementation function in a sequential order according to dependency of the methods. No separate threads are created for each function. 

**retryLimitingHandler function** 

The aim of this function is to set retryLimit to 0 after every 1 second. This helps us in maintaining the desired 2 true values per second property of the normal mode. It takes in index as an argument to know which method retryLimit it has to work on. It works on a separate thread from rest of the program. 

**setDegradationTime function** 

This function is used to set the degradation time for a particular method according to the need of the application. It sets this time randomly in the run time of program and it too works on a separate thread from the rest of the program. 

**stringToInt function**

This method is used to convert a string of numbers into integers. 

**randomSeedSetUp function** 

This method is used to generate a specific seed for each thread before going to allMethodExecutor function to prevent different threads from getting the same values from rand () and hence leading to the desired randomness. 

**retryLogicImplementation function** 

This function applies the exponential back off retry logic for a particular identifier for a particular method. This is where we use the global metrics vector of timeStorage class to get the useful metric time values of the program that could later be used for statistical analysis. 

### Classes 

**Method1, Method2, Method3, Method4, Method5, Method6** 

All these classes are similar whose main motive is to call the modeGuesser method(public), which on taking the identifier returns either true or false. Internally it has two private methods which are called normalMode and degradedMode and modeGuesser calls them randomly using the rand () function. Other desired properties which are needed to be satisfied by these classes are taken in care by an instance of MethodParameters. 

**TimeStorage**

This class is basically used to store the time of completion of identifier, starting and ending time for each attempt for an identifier for all methods. It uses chrono library to store time in a format that is easily convertible to seconds later on. This class also stores the number of attempts it took for a particular identifier to successfully compete the method. 

**MethodParameters**

This class consist of attributes in form of 3 arrays of length 6 storing state, mode and retryLimit for each method. This class aim is to change these parameters of Methods according to the requirement of the program. Thus, it has its required getters and setters. Also, it contains a mutex which helps in synchronized execution of class across multiple threads. 

### Third Model

**Overview**

It has exactly similar setup as the first model just it is extended to incorporate dependent method nature. This is achieved by using a global vector identifierPassed where the following vector restrict the threads for a certain method to work before completion of its dependent method. 

 ![](Images/model3%20framework.png)

**Assumption** 
Dependency of methods are in order 1->2->3->4->5->6.

## Statistics

The Following Site [**statistics**](https://harshitgarg01.github.io/) contains statistics in the form of graph for all the models under 3 different scenarios ->
1. 30 min runtime with degradation state of 60 sec for method2
2. 30 min runtime with degradation state of 540 sec for method2
3. 30 min runtime with degradation state of 900 sec for both method2 and method4 at different times.

Note : It is assumed that model2 and model3 have method dependency in the order 1->2->3->4->5->6.

## Results

Through the graphs for attempts vs identifier for scenario 1 for all the models we can clearly see the effect and benefit of exponential backoff retry logic. When we don't use exponential backoff logic in model1 we see that during the degradation period of 60 sec that begins at 84 we see that number of attempts the identifier can make can go as high as 815 which is substantially higher than what what we see with exponential backoff on the same model where it only goes as high as 31 which even the methods without degradation state might go due to occurance of an odd identifier and randomness of the methods. We can clearly see that in model1 with expo backoff the no of retries attempt got restricted to just 24 for identifier 84 as compared to 815.



## Features: 

1. Model1 does not involve any dependency among methods thus all the methods can run parallel which leads to less identifier invocation time for all the methods. 

2. Model2 and Model3 act as extension of the problem statement while method1 cannot be used in that scenario. 

3. All the Models involve exponential back off logic which though might consume a few more seconds than repeated call of methods but it substantially lowers the retry attempts of method in a degradation state and hence preserving a lot of resources. 

4. Model3 appears to be superior to Model2 in the manner that if a certain method attains a long degradation state than due to parallel computation it does not affect the method proceeding it in case of model3 while sequential arrangement of Method in Model2 completely stops the program. Example if method2 has a degradation state say for 60 sec and method1 is independent of it yet in case of model2 no invocation would be possible while model3 where method1 runs on a separate thread no such stoppage in invocation of method1 would happen. 

5. As number of methods in application are increased then the thread pool for model1 and model3 might become too large which could become tough to manage while no such problem would be there in model2. 

6. Another thing that separates model3 from model1 and model2 is that the identifier passed to methods for execution are in arbitrary order in method3 while they follow a sequential order in model1 and model2.  
