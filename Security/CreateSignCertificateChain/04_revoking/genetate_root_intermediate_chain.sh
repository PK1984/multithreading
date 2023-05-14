#create self signed root certificate
openssl req -x509 -newkey rsa:2048 -keyout root.key -out root.pem -days 365 -noenc -subj "/CN=Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"

#create key and intermediate certificate signing requets
openssl req -new -newkey rsa:2048 -keyout intermediate.key -out intermediate.csr -noenc -subj "/CN=Intermediate CA"
#sign intermediate certificate by the root
openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")

