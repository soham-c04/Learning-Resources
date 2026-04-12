from scapy.all import rdpcap, TCP

pcap_file = "Sniffing.pcapng"
packets = rdpcap(pcap_file)

tls_ports = {443, 8443}  # HTTPS ports

for i, pkt in enumerate(packets):
    if pkt.haslayer(TCP):
        sport = pkt[TCP].sport
        dport = pkt[TCP].dport

        # Check if packet is likely HTTPS
        if sport in tls_ports or dport in tls_ports:
            print("=" * 60)
            print(f"[+] TLS Packet #{i}")

            print(f"Source Port: {sport}")
            print(f"Destination Port: {dport}")

            # Try to detect TLS handshake/content type
            if pkt.haslayer("Raw"):
                raw_data = bytes(pkt["Raw"].load)

                if len(raw_data) > 0:
                    content_type = raw_data[0]

                    if content_type == 22:
                        print("Type: TLS Handshake")
                    elif content_type == 23:
                        print("Type: TLS Application Data (Encrypted)")
                    elif content_type == 21:
                        print("Type: TLS Alert")
                    else:
                        print("Type: Unknown TLS Data")

                    print(f"First 20 bytes: {raw_data[:20]}")
                else:
                    print("No payload data")
            else:
                print("No Raw payload layer")

print("\nScan complete.")
