openssl req -x509 -newkey rsa:2048 -keyout root.key -out root.pem -days 365 -noenc -subj "/CN=Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"

openssl req -new -newkey rsa:2048 -keyout intermediate.key -out intermediate.csr -noenc -subj "/CN=Intermediate CA"
#openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365
openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")
openssl req -new -newkey rsa:2048 -keyout leaf.key -out leaf.csr -noenc -subj "/CN=Leaf Certificate"
openssl x509 -req -in leaf.csr -CA intermediate.pem -CAkey intermediate.key -CAcreateserial -out leaf.pem -days 365 -extfile <(printf "authorityKeyIdentifier=keyid,issuer\nsubjectKeyIdentifier=hash\nkeyUsage=critical,digitalSignature,keyEncipherment\nbasicConstraints=critical,CA:false")
cat leaf.pem intermediate.pem root.pem > chain.pem

openssl req -x509 -newkey rsa:2048 -keyout evil_root.key -out evil_root.pem -days 365 -noenc -subj "/CN=Evil Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"
openssl req -new -newkey rsa:2048 -keyout evil_intermediate.key -out evil_intermediate.csr -noenc -subj "/CN=Evil Intermediate CA"
openssl x509 -req -in evil_intermediate.csr -CA evil_root.pem -CAkey evil_root.key -CAcreateserial -out evil_intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")

