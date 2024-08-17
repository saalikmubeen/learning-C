
In C:

char -> 1 byte
int -> 4 bytes
short -> 2 bytes
float -> 4 bytes
double -> 8 bytes
long -> 8 bytes
pointer -> 4 bytes (32-bit) or 8 bytes (64-bit)

What a 64-bit machine means is that the addresses are 64 bits long(8 bytes). This means that the maximum amount of memory that can be addressed is 2^64 bytes. This is a huge number, and it is unlikely that we will ever have a computer with that much memory. However, it is still useful to have a 64-bit machine because it allows us to address more memory than a 32-bit machine (which can only address 2^32 bytes of memory).
In 64 bit machines at present, the max address you are allowed to use is 2^48 bytes. This is because the remaining 16 bits are used for the sign bit and other flags.

Word Size:
The word size of a computer is the number of bits that the CPU can process at a time. For example, a 32-bit CPU can process 32 bits at a time, and a 64-bit CPU can process 64 bits at a time. The word size of a computer is important because it determines the maximum amount of memory that the computer can address. For example, a 32-bit computer can address 2^32 bytes of memory, while a 64-bit computer can address 2^64 bytes of memory.

Aligned words:
Aligned words refer to the practice of ensuring that data in memory is stored at addresses that are multiples of the word size.
When the data is stored in memory, it is stored in words. A word is the number of bits that the CPU can process at a time. For example, a 32-bit CPU can process 32 bits at a time, so a word on a 32-bit CPU is 32 bits long. When the data is stored in memory, it is stored in words, and the words are aligned in memory. This means that the address of the word in memory is a multiple of the word size. For example, if the word size is 4 bytes, then the address of the word in memory must be a multiple of 4. This is because the CPU can only access memory in word-sized chunks, so the data must be aligned in memory so that the CPU can access it efficiently.
For 64-bit machines, the word size is 8 bytes, so the data must be aligned in memory so that the address of the word in memory is a multiple of 8.

Why Alignment Matters:

Efficiency: CPUs are optimized to fetch and process data that is aligned to word boundaries. Accessing unaligned data can require additional operations to fetch and reassemble the data, resulting in slower performance.
Memory Access: For example, if a CPU has a 4-byte word size (32-bit), aligned data will be stored at memory addresses that are multiples of 4 (e.g., 0x00, 0x04, 0x08, etc.). Unaligned access (e.g., reading 4 bytes starting from address 0x03) might require two memory accesses instead of one, which is less efficient.
Example of Aligned vs. Unaligned:

Aligned: If a 32-bit word (4 bytes) starts at address 0x08, this is aligned because 0x08 is a multiple of 4.
Unaligned: If the same 32-bit word starts at address 0x07, this is unaligned because 0x07 is not a multiple of 4.

Alignment and Hardware:

Some hardware architectures require data to be aligned and may throw exceptions or errors if data is accessed at unaligned addresses. Other architectures may handle unaligned accesses automatically but at a performance cost.

Bit Manipulation:

Bit manipulation can be a powerful tool when working with signed numbers, especially for operations like division by powers of two. When dealing with signed integers, certain considerations need to be made to handle the sign correctly. One common technique involves using a bias to adjust the value before performing the shift operation.

### Division by \(2^n\) using Bit Manipulation

For unsigned numbers, dividing by \(2^n\) is straightforward: you just perform a right shift by \(n\) bits. However, with signed integers (which use two's complement representation), this can introduce errors due to how negative numbers are handled.

### Problem with Simple Right Shift on Signed Numbers

In two's complement representation:

- Right-shifting a positive number by \(n\) bits is equivalent to dividing it by \(2^n\).
- Right-shifting a negative number also performs division by \(2^n\), but it rounds towards negative infinity, not zero. This is because the most significant bit (the sign bit) is extended to the right (arithmetic shift).

### Example

Consider dividing \(-7\) by 4 (\(2^2\)):

- In binary, \(-7\) is represented as `11111001` (assuming an 8-bit signed integer).
- Right-shifting by 2 bits results in `11111110`, which is \(-2\) in decimal (rounded towards negative infinity). However, mathematically, \(-7/4\) should result in \(-1\) (rounded towards zero).

### The Bias Technique

To correctly handle the division of signed integers by powers of two, we can use a **bias**. The bias helps to correct the rounding behavior when shifting negative numbers.

#### Bias Calculation

The bias is typically calculated as:

\[
\text{bias} = 2^n - 1
\]

This bias is added to the number before performing the shift.

### Steps to Perform Signed Division by \(2^n\)

1. **Check Sign**: If the number is negative, add the bias before shifting. If the number is positive, no bias is needed.

2. **Add Bias** (for negative numbers): The bias ensures that when the number is right-shifted, the result is rounded towards zero rather than negative infinity.

3. **Right Shift**: Perform the right shift by \(n\) bits.

#### Formula

For a signed integer \(x\) and division by \(2^n\):

\[
\text{Result} = (x + \text{bias}) >> n
\]

Where:

- \(\text{bias} = (1 << n) - 1\) if \(x\) is negative,
- \(\text{bias} = 0\) if \(x\) is positive.

### Example

Let's take \(-7\) and divide by 4 (\(n = 2\)):

1. **Calculate Bias**:
   - \(\text{bias} = 2^2 - 1 = 3\)

2. **Add Bias and Shift**:
   - \(\text{Result} = (-7 + 3) >> 2\)
   - \(-7 + 3 = -4\)
   - \(-4 >> 2 = -1\) (correctly rounds towards zero)

### Summary

- **Bit Manipulation**: Right-shifting by \(n\) is a fast way to divide by \(2^n\).
- **Signed Numbers**: Bias is used to correct the division result for negative numbers, ensuring it rounds towards zero.
- **Practical Use**: This method is often used in low-level programming and optimization where performance is critical, and division by powers of two is frequent.

Floating-point numbers in computers are represented using a format based on the IEEE 754 standard, which is the most widely adopted standard for floating-point arithmetic. This format allows computers to represent a wide range of real numbers, including very small and very large values, by using a scientific notation-like approach.

### IEEE 754 Format

A floating-point number is represented by three main components:

1. **Sign Bit (S)**: 1 bit that indicates the sign of the number.
   - `0` represents a positive number.
   - `1` represents a negative number.

2. **Exponent (E)**: A set of bits that represents the exponent (which can be positive or negative) of the number.

3. **Mantissa (M)** or **Significand**: This represents the significant digits of the number.

The general form of a floating-point number in binary is:

\[ \text{value} = (-1)^{S} \times 1.M \times 2^{(E - \text{Bias})} \]

### Detailed Representation in IEEE 754

#### Single Precision (32-bit)

- **Sign Bit (S)**: 1 bit
- **Exponent (E)**: 8 bits
- **Mantissa (M)**: 23 bits

The exponent has a bias of 127 (i.e., the actual exponent is stored as \( E - 127 \)).

#### Double Precision (64-bit)

- **Sign Bit (S)**: 1 bit
- **Exponent (E)**: 11 bits
- **Mantissa (M)**: 52 bits

The exponent has a bias of 1023 (i.e., the actual exponent is stored as \( E - 1023 \)).

### Example: Representing 9.375 in IEEE 754 Single Precision

#### Step 1: Convert the Number to Binary

First, break the number into its integer and fractional parts:

- Integer part: `9` in binary is `1001`.
- Fractional part: `0.375` in binary is `0.011`.

So, \( 9.375_{10} = 1001.011_2 \).

#### Step 2: Normalize the Binary Number

Convert the binary number to scientific notation in base 2:

\[ 1001.011_2 = 1.001011 \times 2^3 \]

Here, the exponent is `3`, and the mantissa is `001011` (excluding the leading 1).

#### Step 3: Determine the Sign, Exponent, and Mantissa

- **Sign Bit (S)**: `0` (since 9.375 is positive).
- **Exponent (E)**: Add the bias to the actual exponent (3):
  - In single precision, the bias is 127.
  - \( E = 3 + 127 = 130 \).
  - `130` in binary is `10000010`.
- **Mantissa (M)**: The fractional part `001011`, padded with zeros to fill 23 bits:
  - `00101100000000000000000`.

#### Step 4: Assemble the Bits

The final 32-bit representation of `9.375` in IEEE 754 single precision is:

```
Sign    Exponent (E)     Mantissa (M)
0       10000010         00101100000000000000000
```

So, in hexadecimal, this would be represented as:

`0x41160000`

Let's break down the concept of floating-point representation with a detailed example, using both single-precision (32-bit) and double-precision (64-bit) formats. We'll represent the number 9.375 in both formats to illustrate how floating-point numbers are stored in a computer.

### Example 1: Single Precision (32-bit)

#### Step 1: Convert 9.375 to Binary

1. **Convert the integer part (9) to binary:**
   - \( 9_{10} = 1001_2 \)

2. **Convert the fractional part (0.375) to binary:**
   - Multiply 0.375 by 2:
     \( 0.375 \times 2 = 0.75 \) → `0` (carry `0.75`)
   - Multiply 0.75 by 2:
     \( 0.75 \times 2 = 1.5 \) → `1` (carry `0.5`)
   - Multiply 0.5 by 2:
     \( 0.5 \times 2 = 1.0 \) → `1` (carry `0.0`)

   So, \( 0.375_{10} = 0.011_2 \).

3. **Combine the integer and fractional parts:**
   - \( 9.375_{10} = 1001.011_2 \)

#### Step 2: Normalize the Binary Number

To normalize \( 1001.011_2 \):

\[ 1001.011_2 = 1.001011 \times 2^3 \]

- The exponent is `3`.
- The mantissa is `001011` (the bits after the `1.`).

#### Step 3: Determine the Sign, Exponent, and Mantissa

- **Sign Bit (S)**: `0` (since 9.375 is positive).
- **Exponent (E)**: Add the bias (127) to the exponent (3):
  - \( E = 3 + 127 = 130 \)
  - \( 130_{10} = 10000010_2 \)
- **Mantissa (M)**: Take `001011` and pad with zeros to make 23 bits:
  - `00101100000000000000000`

#### Step 4: Assemble the 32-bit Representation

The final 32-bit IEEE 754 representation of 9.375 is:

```
Sign    Exponent (E)     Mantissa (M)
0       10000010         00101100000000000000000
```

In hexadecimal, this is represented as:

- `0x41160000`

### Example 2: Double Precision (64-bit)

Now, let's represent the same number, 9.375, in double precision.

#### Step 1: Convert 9.375 to Binary

This step is the same as in the single-precision example, yielding:

- \( 9.375_{10} = 1001.011_2 \)

#### Step 2: Normalize the Binary Number

- Normalize to: \( 1.001011 \times 2^3 \)
- The exponent is still `3`.
- The mantissa is `001011`.

#### Step 3: Determine the Sign, Exponent, and Mantissa

- **Sign Bit (S)**: `0`
- **Exponent (E)**: Add the bias (1023) to the exponent (3):
  - \( E = 3 + 1023 = 1026 \)
  - \( 1026_{10} = 10000000010_2 \)
- **Mantissa (M)**: The mantissa `001011`, padded with zeros to make 52 bits:
  - `0010110000000000000000000000000000000000000000000000`

#### Step 4: Assemble the 64-bit Representation

The final 64-bit IEEE 754 representation of 9.375 is:

```
Sign    Exponent (E)        Mantissa (M)
0       10000000010         0010110000000000000000000000000000000000000000000000
```

In hexadecimal, this is represented as:

- `0x4026400000000000`

### Summary of Examples

1. **Single Precision (32-bit) for 9.375**:
   - Binary: `0 10000010 00101100000000000000000`
   - Hexadecimal: `0x41160000`

2. **Double Precision (64-bit) for 9.375**:
   - Binary: `0 10000000010 0010110000000000000000000000000000000000000000000000`
   - Hexadecimal: `0x4026400000000000`

### Summary

In summary, floating-point numbers are represented using the IEEE 754 standard, which breaks down a number into a sign bit, an exponent, and a mantissa. This representation allows for a wide range of values and is efficient for performing arithmetic operations on real numbers in computers.

Let's delve deeper into the process of converting the fractional part of a decimal number to its binary equivalent. We'll use the fractional part `0.375` as the example.

### Step-by-Step Conversion of 0.375 to Binary

The process of converting a fractional decimal number to binary involves multiplying the fraction by 2 and recording the integer part of the result. This is repeated with the fractional part until the fractional part becomes 0 (or until you reach the desired level of precision).

#### Initial Fractional Part: 0.375

1. **Multiply 0.375 by 2:**
   \[
   0.375 \times 2 = 0.75
   \]
   - **Integer Part:** 0
   - **Fractional Part:** 0.75

   The first digit of the binary fraction is `0`.

2. **Multiply the fractional part (0.75) by 2:**
   \[
   0.75 \times 2 = 1.5
   \]
   - **Integer Part:** 1
   - **Fractional Part:** 0.5

   The second digit of the binary fraction is `1`.

3. **Multiply the fractional part (0.5) by 2:**
   \[
   0.5 \times 2 = 1.0
   \]
   - **Integer Part:** 1
   - **Fractional Part:** 0.0

   The third digit of the binary fraction is `1`.

   Since the fractional part is now 0, we can stop here.

### Combine the Results

The binary digits collected from each step give the binary representation of the fractional part:

- The first multiplication gave us `0`.
- The second multiplication gave us `1`.
- The third multiplication gave us `1`.

So, \( 0.375_{10} \) is represented as \( 0.011_2 \) in binary.

### Summary

- **0.375 in decimal** = **0.011 in binary**.

Each step involves multiplying the fractional part by 2, and the integer part of the result becomes the next digit in the binary fraction. This method is applicable to any fractional decimal number and allows you to convert it accurately into binary form.

When converting a fractional decimal number to binary, sometimes the fraction results in a repeating binary sequence, much like how certain decimal fractions (like \( \frac{1}{3} = 0.333\ldots \)) have repeating decimal sequences.

### Example: Converting 0.1 to Binary

Let's convert \( 0.1_{10} \) (0.1 in decimal) to binary.

#### Step-by-Step Conversion of 0.1 to Binary

1. **Multiply 0.1 by 2:**
   \[
   0.1 \times 2 = 0.2
   \]
   - **Integer Part:** 0
   - **Fractional Part:** 0.2

   The first digit of the binary fraction is `0`.

2. **Multiply the fractional part (0.2) by 2:**
   \[
   0.2 \times 2 = 0.4
   \]
   - **Integer Part:** 0
   - **Fractional Part:** 0.4

   The second digit of the binary fraction is `0`.

3. **Multiply the fractional part (0.4) by 2:**
   \[
   0.4 \times 2 = 0.8
   \]
   - **Integer Part:** 0
   - **Fractional Part:** 0.8

   The third digit of the binary fraction is `0`.

4. **Multiply the fractional part (0.8) by 2:**
   \[
   0.8 \times 2 = 1.6
   \]
   - **Integer Part:** 1
   - **Fractional Part:** 0.6

   The fourth digit of the binary fraction is `1`.

5. **Multiply the fractional part (0.6) by 2:**
   \[
   0.6 \times 2 = 1.2
   \]
   - **Integer Part:** 1
   - **Fractional Part:** 0.2

   The fifth digit of the binary fraction is `1`.

6. **Multiply the fractional part (0.2) by 2:**
   \[
   0.2 \times 2 = 0.4
   \]
   - **Integer Part:** 0
   - **Fractional Part:** 0.4

   The sixth digit of the binary fraction is `0`.

Now notice that the fractional part has cycled back to `0.4`, meaning the sequence will start repeating from here.

### Combine the Results

The binary digits collected are:

- \( 0.1_{10} \) in binary is \( 0.0001100110011\ldots_2 \), where `0011` repeats indefinitely.

### Repeating Binary Fraction

The repeating part in the binary fraction can be indicated using a bar notation (just like in repeating decimals). Therefore, you can represent \( 0.1_{10} \) as:

\[
0.1_{10} = 0.0001100110011\ldots_2 = 0.0001\overline{1001}_2
\]

### Summary

- **Repeating Fraction**: When converting fractions like 0.1 to binary, you may end up with a repeating sequence of bits.
- **Representation**: This can be represented using bar notation to indicate the repeating portion, just as in repeating decimals.

### Real-World Implications

Due to the repeating nature of some fractions in binary, exact representation may not be possible with a finite number of bits, leading to small rounding errors in floating-point arithmetic. This is why some numbers like 0.1 cannot be represented exactly in binary floating-point formats, which can lead to small inaccuracies in calculations.
