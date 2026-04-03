# My Uni Work

This repository contains a selection of major projects completed during my university degree.  
Only large-scale coursework is included here — not small exercises or minor submissions.  

The projects cover different areas such as mobile development, web applications, databases, data processing and visualization and more. This repository also serves as a portfolio of my academic work.  


## Second Year

### Android Studio App

Android application developed in Android Studio as part of a university course project. 
The objective was to design and implement a booking system for a business offering court reservations. 

RxJava was used for the development of the application, which includes features such as user registration, 
login, and court booking.

### Concurrent Programming 

A C project for concurrent processing of global temperature datasets. It includes an optimized code using multithreading (pthreads), mutex synchronization, and the producer-consumer pattern to efficiently compute country-wise temperature extremes.

### Embedded systems

This project implements embedded control algorithms for robotics applications using the C programming language on the MSP432 microcontroller platform. The focus is on communication with sensors, motor control via UART in half-duplex mode, and implementing basic movement states such as straight driving, wall following, and rotation.

The code includes UART initialization, half-duplex communication handling, and state management to enable the robot to navigate in different scenarios.

### Linear interpolation and numerical algebra

This project implements fundamental scientific computing methods using the C programming language. It focuses on two main areas:  Linear interpolation to approximate functions based on discrete data points and Numerical linear algebra, including solving systems of linear equations using techniques such as Gaussian elimination.

# Natural Disasters Database Project

This repository contains the full development of a two-part academic project focused on building and analyzing a database related to natural disasters. The goal was to define a real-world data model, collect and process relevant information, and perform exploratory and analytical queries using relational algebra and SQL. The first part covers the theoretical and conceptual design (including the ER model and query planning), while the second part focuses on the database implementation and data analysis. The project was developed as part of the "Database Theory" course at the University of Barcelona (2023–2024).


## Third Year

### Artificial Vision

This repository contains a series of practical labs focused on key concepts in image processing and computer vision in python. 
The labs cover a wide range of topics, including image loading, manipulation, and color processing, as well as more advanced techniques like edge detection, image segmentation, and object detection. 
Methods such as Gaussian filters, template matching, and feature extraction (ORB, HOG), along with face detection using Haar-like features and PCA. These practicum exercises were designed to provide hands-on experience in fundamental image processing, 
filtering, and object recognition.

### Data Science Mini Projects

This repository contains three projects developed as part of the "Tallers de Nous Usos de la Informàtica (TNUI)" course at the University of Barcelona. 
The projects showcase different aspects of data science and machine learning, including exploratory data analysis (EDA), recommender systems, and classification using machine learning algorithms. 
Each project utilizes real-world datasets to apply advanced Python tools and techniques.

### Distributed Client Server Game

This project consists of building a distributed version of the classic Battleship game entirely from scratch using Java. 
The application follows a client-server architecture, where two players connect through sockets to play in real time. 
One instance runs as the server, handling the game logic and coordination, while the other acts as the client, allowing a second player to join and interact with the server.

The main goal of the assignment was to understand the fundamentals of network communication using Java sockets, while also managing synchronization, message exchange, and game state between two separate processes. 
All communication is done over TCP, and the game supports turn-based interaction, shot validation, and game-over detection.

This practice provides hands-on experience with distributed systems, real-time interaction, and Java's low-level networking APIs.

### Full Stack Game 

This project is a web-based implementation of the classic Battleship game, developed using a full-stack architecture. The frontend is built with the JavaScript framework Vue.js, providing an interactive and visually dynamic interface where players can place ships, view the game board in real time, and play turns directly in the browser. It acts as the client, running on the user’s machine within modern web browsers like Chrome, Firefox, or Safari.

The backend is developed using the Django framework in Python, designed to handle game logic, user authentication, and data persistence. The backend exposes a RESTful API using Django Rest Framework (DRF), and leverages Django Auth for user management and drf-spectacular for automated API documentation. Data models are managed through Django’s ORM system and synchronized with a SQLite database using migrations. This architecture separates concerns between client interaction and server logic, offering a scalable and modular approach to building multiplayer web applications.

### Raytracing

This project is a custom-built graphics engine in C++ developed to explore and visualize core ray-based rendering techniques. 
Initially based on a simple GUI that displayed only a solid background color, the engine has been extended to support raycasting, raytracing, and path tracing. 
It also features optical effects such as transparency, metallic reflections (mirror surfaces), and texture mapping.

In addition to geometry and lighting, the engine incorporates multiple shading models to simulate different visual styles, including normal shading, diffuse shading, Blinn-Phong, and toon shading. 
The main goal is to provide a hands-on understanding of how various rendering and shading techniques interact to produce realistic and stylized images in computer graphics.

### Z-Buffering

This project is a custom-built graphics engine in C++ developed to explore and demonstrate core rasterization-based rendering techniques using a traditional z-buffer pipeline.
Initially built with a simple GUI that displayed only a solid background color, the engine has been expanded to support scene transformations, per-object manipulation, multiple lighting types, and various shading models executed on the GPU.

The engine includes support for lighting techniques such as ambient and point lights, GPU-based material handling, and several shading models including normal shading, Phong, Gouraud, toon shading, and silhouette emphasis.
Additionally, it features support for texturing, texture-per-object assignment, indirect mapping, and optional non-photorealistic effects.

The main goal is to provide a hands-on understanding of how the rasterization pipeline operates and how different lighting and shading models can be combined to produce both realistic and stylized visual outputs in real-time graphics.


### Globe_gl

This project visualizes global country data on a 3D interactive globe using geolocated datasets (countries.json and area_country.geojson). Each country is represented by a point with scalar and categorical attributes like currency, region, timezone, and language. 
The visualization supports intuitive exploration of this structured, static dataset through geographical and attribute-based patterns.

Three visual techniques are implemented: individual point markers with tooltips for precise country data lookup, color-coded areas by region or attribute (e.g. currency) for pattern comparison, and elevated outlines to highlight selected countries. 
These combined methods allow for efficient exploration, comparison, and contextual understanding of diverse global information.


### Reinforcement Learning

In this practical, we explore Q-learning, a fundamental reinforcement learning algorithm, applied to both a simple grid navigation task and a simplified chess scenario. We evaluate performance under deterministic and stochastic conditions, analyze convergence behavior, and compare results with traditional search algorithms.

### User centered design web

This is a smart baking time-management web app designed to help you plan, organize, and bake multiple recipes at once. Simply add your favorite recipes, and the app will generate a synchronized timeline so you can prepare and bake efficiently. It minimizes waiting time and optimizes oven usage. Perfect for busy bakers managing several treats at the same time.


Please note that this app is a functional prototype with a simulated backend. It was developed as part of a course focused on user-centered design, where the main objective was to deeply understand user needs and create meaningful solutions. The emphasis was placed on research, ideation, and prototyping rather than building a fully operational web application.



## Fourth Year

### Machine Learning

This repository contains a set of practical projects covering the full machine learning workflow, from data exploration to model evaluation and optimization using Python and scikit-learn.  

The projects include regression models (Linear Regression, KNN), classification with Support Vector Machines (SVM), and ensemble methods such as boosting implemented from scratch using Decision Trees.  
Advanced topics such as handling imbalanced datasets, evaluation metrics, overfitting, and hyperparameter tuning are also explored.  

Additionally, a deep learning project using Convolutional Neural Networks (CNNs) was developed to process and analyze image data, including architecture design, data augmentation, and regularization techniques.  




### Software Engineering

This repository contains a full-stack project developed in a team of 7 people, simulating a real-world software development environment using Agile and DevOps methodologies.  

The project consists of an event management system built from scratch with a React frontend and a Spring Boot backend, connected to a PostgreSQL database (Supabase) and deployed using CI/CD pipelines on Render.  

The development process followed Scrum with multiple sprints, including planning, daily stand-ups, demos, and retrospectives. Tasks were managed using Kanban boards, and all features were defined through user stories with acceptance criteria.  

A key aspect of the project was role rotation, where each team member worked across different roles such as frontend, backend, QA, DevOps, Scrum Master, and Product Owner.  

The project also incorporated professional practices such as GitHub-based workflows, code reviews, automated testing, and continuous integration and deployment.  

**Tools & Technologies:** React, Java, Spring Boot, PostgreSQL (Supabase), GitHub, GitHub Actions, Render, REST APIs, Agile (Scrum, Kanban)  



### Image Processing

This repository focuses on advanced image processing techniques, with an emphasis on image restoration and manipulation tasks.  

The projects include methods such as image inpainting, filtering, and reconstruction, exploring how missing or corrupted regions of an image can be recovered using algorithmic approaches. The work combines mathematical foundations with practical implementations to understand how images can be enhanced, repaired, or transformed.  

The main goal is to gain a deeper understanding of how low-level image processing techniques can be applied to real-world problems, bridging the gap between classical computer vision and modern machine learning approaches.  
