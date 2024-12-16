Here’s the updated **README** without the Docker Hub push instructions:

---

# **Optimized Price Point Book**

> [!NOTE]  
> **This project was completed in 3 days as part of an interview task.**

---

## **Task Overview**

The goal of this project is to efficiently maintain and benchmark an **order book** data structure. The order book holds buy (bids) and sell (asks) orders at various price levels. The challenge is to achieve **high performance** in terms of:
- Insertion and removal of orders.
- Fetching the best bid (highest buy price) and best ask (lowest sell price).

---

## **Implementations**

Two key implementations of the order book were developed:

1. **`std::map` Implementation**
   - Uses a **Red-Black Tree** (`std::map`) for maintaining sorted bids and asks.
   - Time Complexity:
     - **Insertion**: O(log N)
     - **Lookup for best bid/ask**: O(1)

2. **Fixed `std::array` + Binary Search Implementation**
   - Uses a **fixed-size array** for storage and **binary search** to maintain sorted order.
   - Time Complexity:
     - **Insertion**: average case O(log N); worst case - O(N)
     - **Lookup for best bid/ask**: O(1)

### **Benchmarking**
The performance of both implementations is measured and compared based on:
- **Memory usage**.
- **Execution time** for order insertions and queries.

---

## **Local Build Instructions**

### **Prerequisites**
- `CMake` (version 3.14 or higher).
- `g++` compiler (C++20 support).
- Boost library (JSON, System, Thread components).
- `libssl-dev`.

### **Steps**
1. Clone the repository:

   ```bash
   git clone <repo_url>
   cd <repo_directory>
   ```

2. Create a **build directory** and run CMake:

   ```bash
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   ```

3. Compile the project:

   ```bash
   make -j$(nproc)
   ```

4. Executables will be available in the `build` directory.

---

## **How to Run**

### **Benchmark**
To run the benchmark comparing the two implementations:

```bash
./build/generate_data   # Generates data files (e.g., snapshot and updates).
./build/benchmark       # Runs the benchmark tests.
```

### **Unit Tests**
To execute unit tests:

```bash
./build/tests
```

### **Binance Listener (Out of Scope)**
The Binance live listener is implemented but incomplete. For now, it can be compiled and executed as follows:

```bash
./build/binance_listener
```

---

## **Docker Build and Run**

### **Build Docker Image**
To build the Docker image locally:

```bash
docker build -t orderbook-app .
```

### **Run Benchmarks Inside Docker**

Run the data generation and benchmarks in a single command:

```bash
docker run --rm orderbook-app "./generate_data && ./benchmark"
```

### **Run Unit Tests**
Execute unit tests:

```bash
docker run --rm orderbook-app ./tests
```

---

## **TODO**

- Finalize the **Binance live connection** (`binance_listener`) for real-time order book updates.
- Add support for **multiple threads** to process different symbols concurrently.
- Implement double to integer conversion so the prices can be compared quickly
