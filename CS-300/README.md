


# Course Planner

- What was the problem you were solving in the projects for this course?

This program is designed to display a course roster, validating courses dependancies, while provided a seaerch feature.

- How did you approach the problem? Consider why data structures are important to understand.

The program specifically gathers all the courses to allowing them to be sorted by means of a vector list. With the current state of the software the advisors are using it to view a list of courses in the different orders.  The list is not being altered during program use only as a viewing model.  With this a flat list provided by a vector will be less resource intensive for altering to the needs of the user.  The benefit to this there are many built in functions available for vectors that will reduce overhead of additional functionality leaving an average time complexity of O(N).  Other data types will provide a better time complexity for accessing and searching but will require extra overhead to create a sort-able flat list.  Tests were performed over the weeks to assess different data structure to ensure the best was used for the needs of the users.  Particularily noting that Binary search trees and has tables provided quicker times the features these data structures proived would not be heavily used, future expansion to this program will allow the need these feature while the current code can be maintined with the data exported to a flat list. 

- How did you overcome any roadblocks you encountered while going through the activities or project?

Roadblocks hit while creating this program were resolved by multiple methods, primarily by debugging with break points assessing each step in the logic provided.  Next would be by researching approaches to the logic attempted to determine the needs for the logic to succeed.  

- How has your work on this project expanded your approach to designing software and developing programs?

This project particularily opened my eyed to the different uses data structures can provide along with methods of sorting large amounts of datasets.  While I know there are more methods out there to complete this, what provided here works well as a starting point for further research.

- How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

Internal commenting and organization of the code allows for the code to be improved upon in small sections at a time.  This will enable easier alteration to the code, allowing for a modular approach with changes made.  With goal to keep functions a bit more objective for easier refractoring.


### Additional Project Files:
- [Runtime Analysis](Runtime_Analysis.docx)