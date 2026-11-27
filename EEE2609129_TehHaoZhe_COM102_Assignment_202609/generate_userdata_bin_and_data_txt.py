import struct

# --- System Rules (written to data.txt and used to calculate citizen taxes) ---
brackets = [
    (0.0, 48000.0, 0.00),
    (48000.0, 60000.0, 0.05),
    (60000.0, 100000.0, 0.07),
    (100000.0, 200000.0, 0.09),
    (200000.0, 500000.0, 0.14),
    (500000.0, 1000000.0, 0.19),
    (1000000.0, 1.7976931348623157e+308, 0.25),
]

child_table = [
    ("Child allowance A", 1, 2, 8000.0),
    ("Child allowance B", 3, 4, 10000.0),
    ("Child allowance C", 5, 6, 12000.0),
    ("Child allowance D", 7, 99, 14000.0),
]

deduction_table = [
    ("Marital allowance", 5000.0),
    ("Special healthcare", 20000.0),
]

# --- The 20 Citizen Records ---
records = [
    # (ID, Name, Password, Age, MaritalStatus, AnnualIncome, Children, ChildrenU18, SpecialHealth)
    ("100000000000", "Josh",     "pass01", 28, 0,  32000.00, 0, 0, 0),
    ("100000000001", "Mark",     "pass02", 31, 0,  48000.00, 0, 0, 0),
    ("100000000002", "Nolan",    "pass03", 34, 1,  53000.00, 0, 0, 0),
    ("100000000003", "Amelia",   "pass04", 38, 0,  55000.00, 0, 0, 0),
    ("100000000004", "Cyrene",   "pass05", 36, 1,  73000.00, 1, 1, 0),
    ("100000000005", "Tebia",    "pass06", 41, 1,  88000.00, 2, 2, 0),
    ("100000000006", "Phinon",   "pass07", 43, 1, 115000.00, 3, 3, 0),
    ("100000000007", "Agelia",   "pass08", 46, 1, 145000.00, 4, 4, 0),
    ("100000000008", "Castoric", "pass09", 47, 1, 177000.00, 5, 5, 0),
    ("100000000009", "Cydere",   "pass10", 50, 1, 237000.00, 6, 6, 1),
    ("100000000010", "Chiper",   "pass11", 52, 1, 299000.00, 7, 7, 1),
    ("100000000011", "Hysine",   "pass12", 54, 1,  95000.00, 3, 2, 0),
    ("100000000012", "Hyacine",  "pass13", 27, 0,  22000.00, 0, 0, 1),
    ("100000000013", "Anaxa",    "pass14", 30, 1,  18000.00, 2, 2, 1),
    ("100000000014", "Mydei",    "pass15", 39, 0, 500000.00, 0, 0, 0),
    ("100000000015", "DanHeng",  "pass16", 45, 1, 685000.00, 2, 2, 1),
    ("100000000016", "March",    "pass17", 51, 0, 1000000.00, 0, 0, 0),
    ("100000000017", "Stelle",   "pass18", 56, 1, 1539000.00, 8, 8, 1),
    ("100000000018", "Himeko",   "pass19", 37, 0,  90000.00, 2, 1, 0),
    ("100000000019", "Welt",     "pass20", 33, 1, 133000.00, 2, 2, 0),
]

# --- 1. Generate data.txt ---
def create_data_txt():
    with open("data.txt", "w", encoding="utf-8") as f:
        f.write(f"{len(brackets)} {len(child_table)} {len(deduction_table)}\n")
        for mn, mx, rate in brackets:
            mx_str = "1.7976931348623157E+308" if mx > 1e300 else f"{mx:.1f}"
            f.write(f"{mn:.1f}, {mx_str}, {rate:.2f}\n")
        for cat, mn, mx, amt in child_table:
            f.write(f'"{cat}", {mn}, {mx}, {amt:.1f}\n')
        for cat, amt in deduction_table:
            f.write(f'"{cat}", {amt:.1f}\n')
    print(f"data.txt created successfully! ({len(brackets)} brackets, {len(child_table)} child allowances, {len(deduction_table)} deductions)")


# --- 2. Generate userdata.bin ---
# Matches struct Citizen in Assignment_1_3.c (maritalStatus & specialHealthcare are 4-byte ints)
CITIZEN_FMT = "<13s50s20s1xiiidiii4xddd3i4xd"
CITIZEN_SIZE = struct.calcsize(CITIZEN_FMT)  # 168 bytes
MAX_CITIZENS = 30

def pack_citizen(rec):
    cid, name, pwd, age, mar, inc, kids, u18, hc = rec
    login_cnt = 2  # login count >= 2 so it behaves as an established citizen

    tot_ded = 0.0
    if mar:
        tot_ded += deduction_table[0][1]

    c_ded = 0.0
    child_cat = 0
    if kids > 0 and kids == u18:
        for idx, (_, mn, mx, amt) in enumerate(child_table):
            if mn <= kids <= mx:
                c_ded = amt
                child_cat = idx + 1
                break
    tot_ded += c_ded

    # Match C logic: special healthcare only valid if married or has children
    if hc and (mar or kids > 0):
        tot_ded += deduction_table[1][1]
    else:
        hc = 0

    taxable_inc = max(0.0, inc - tot_ded)
    tax_rate = brackets[0][2] if taxable_inc <= brackets[0][1] else 0.0
    if taxable_inc > brackets[0][1]:
        for mn, mx, r in brackets[1:]:
            if mn < taxable_inc <= mx:
                tax_rate = r
                break
    inc_tax = taxable_inc * tax_rate

    return struct.pack(
        CITIZEN_FMT,
        cid.encode("ascii"), name.encode("ascii"), pwd.encode("ascii"),
        login_cnt, age, int(mar), inc, kids, u18, int(hc),
        tot_ded, taxable_inc, inc_tax,
        int(mar), child_cat, int(hc), tot_ded
    )

def create_userdata_bin():
    bin_data = bytearray()
    bin_data.extend(struct.pack("<i", len(records)))
    for r in records:
        bin_data.extend(pack_citizen(r))
    bin_data.extend(b"\x00" * (CITIZEN_SIZE * (MAX_CITIZENS - len(records))))

    with open("userdata.bin", "wb") as f:
        f.write(bin_data)
    print(f"userdata.bin created successfully! ({len(bin_data)} bytes, {len(records)} citizens)")


if __name__ == "__main__":
    create_data_txt()
    create_userdata_bin()