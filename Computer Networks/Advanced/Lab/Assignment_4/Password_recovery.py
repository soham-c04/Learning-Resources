import hashlib

actual_password = "10"
stored_hash = hashlib.sha256(actual_password.encode()).hexdigest()

print(f"Target Hash: {stored_hash}\n")

# ===== Step 2: Generate all 2-digit passwords (00–99) =====
for i in range(100):
    password = f"{i:02d}"

    # ===== Step 3: Hash the generated password =====
    hashed = hashlib.sha256(password.encode()).hexdigest()

    print(f"Trying: {password} -> {hashed}")

    # ===== Step 4: Compare with stored hash =====
    if hashed == stored_hash:
        print("\n[+] Password Found!")
        print(f"Password is: {password}")
        break
else:
    print("\n[-] Password not found in range 00–99")
