from scapy.all import rdpcap, TCP, Raw
import re

packets = rdpcap("Sniffing.pcapng")

for pkt in packets:
    if pkt.haslayer(TCP) and pkt.haslayer(Raw):
        try:
            payload = pkt[Raw].load.decode(errors='ignore')
        except:
            continue

        if "POST" in payload:

            host_match = re.search(r"Host:\s*(.*?)\r\n", payload)
            path_match = re.search(r"POST\s+(.*?)\s+HTTP", payload)

            if host_match and path_match:
                url = "http://" + host_match.group(1) + path_match.group(1)
                print("\nURL:", url)

            parts = payload.split("\r\n\r\n")
            if len(parts) > 1:
                form_data = parts[1]

                creds = re.findall(r"(\w+)=([^&]+)", form_data)
                if creds:
                    print("\n-------- Extracted Fields --------")
                    for key, value in creds:
                        print(f"{key} = {value}")

                    print("="*40)
