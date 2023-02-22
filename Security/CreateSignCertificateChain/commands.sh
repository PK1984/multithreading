openssl req -x509 -newkey rsa:2048 -keyout root.key -out root.pem -days 365 -subj "/CN=Root CA"
openssl req -new -newkey rsa:2048 -keyout intermediate.key -out intermediate.csr -subj "/CN=Intermediate CA"
#openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365
openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")
openssl req -new -newkey rsa:2048 -keyout leaf.key -out leaf.csr -subj "/CN=Leaf Certificate"
openssl x509 -req -in leaf.csr -CA intermediate.pem -CAkey intermediate.key -CAcreateserial -out leaf.pem -days 365
cat root.pem intermediate.pem leaf.pem > chain.pem
