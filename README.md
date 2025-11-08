# Shopping Portal using Stack Implementation

This project is submitted as a partial requirement for the course **CS-2001** in Fall 2025.

## Team Members
- **Hashir Ali Butt (k24-0694)**
- **Abdul Wasay (k24-0744)**
- **Muhammad Saad (k24-0589)**

---

## Project Goal

Design and implement an online shopping portal that utilizes stack implementation through linked lists to efficiently manage a user’s shopping cart.

---

## Project Description

The proposed Shopping Portal system allows users to:
- Add, remove, and view products in their cart using stack operations.
- Store each product as a node in a linked list containing:
  - Product name
  - Price
  - Pointer to the next product
- Simulate a stack-based shopping experience with:
  - Addition of products to the cart (push)
  - Removal of the most recently added product (pop)
- View the entire cart along with the total cost
- Restrict cart size to ensure efficient memory management

**This project demonstrates the use of**:
- Data structures (Stacks and Linked Lists)
- Real-world applications such as online shopping

---

## System Actors

- **Customer/User**
  - Adds/removes products
  - Views total cost

- **Admin**
  - Manages available products
  - Enforces product limits

---

## Features & Applied Data Structures/Algorithms

1. **Add Product to Cart**  
   - Stack: Push operation using Linked List
2. **Delete Product from Cart**  
   - Stack: Pop operation using Linked List
3. **View Last Added Product**  
   - Stack: Top access
4. **Display Product List & Total Cost**  
   - Linked List: Traversal
5. **Limit Cart Size**  
   - Conditional check before push operation

---

## Tools & Techniques

- Developed in **C++**
- Stacks implemented using **Linked Lists**
- Primary IDE: **Visual Studio Code**

**Key concepts applied:**
- Dynamic Memory Allocation
- Structures
- Pointers
- Stack Operations
- Linked List Traversal

---

## Usage

\```bash
# Compile using g++ (example)
g++ -o ShoppingPortal main.cpp
./ShoppingPortal
\```

---

## License

This project is for academic use in the CS-2001 course.
