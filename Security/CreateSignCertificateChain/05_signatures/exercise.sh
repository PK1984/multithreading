openssl req -x509 -newkey rsa:2048 -keyout root.key -out root.pem -days 365 -noenc -subj "/CN=Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"

openssl dgst -sha256 -sign root.key -out signature.bin message.txt
openssl x509 -in root.pem -pubkey -noout > root_public_key.pem
openssl dgst -sha256 -verify root_public_key.pem -signature signature.bin message.txt #verified OK
