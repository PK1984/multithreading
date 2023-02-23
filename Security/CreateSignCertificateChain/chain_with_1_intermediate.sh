openssl req -x509 -newkey rsa:2048 -keyout root.key -out root.pem -days 365 -noenc -subj "/CN=Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"

openssl req -new -newkey rsa:2048 -keyout intermediate.key -out intermediate.csr -noenc -subj "/CN=Intermediate CA"
#openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365
openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")
openssl req -new -newkey rsa:2048 -keyout leaf.key -out leaf.csr -noenc -subj "/CN=Leaf Certificate"
openssl x509 -req -in leaf.csr -CA intermediate.pem -CAkey intermediate.key -CAcreateserial -out leaf.pem -days 365 -extfile <(printf "authorityKeyIdentifier=keyid,issuer\nsubjectKeyIdentifier=hash\nkeyUsage=critical,digitalSignature,keyEncipherment\nbasicConstraints=critical,CA:false")
cat root.pem intermediate.pem leaf.pem > chain.pem

openssl req -x509 -newkey rsa:2048 -keyout evil_root.key -out evil_root.pem -days 365 -noenc -subj "/CN=Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"
openssl req -new -newkey rsa:2048 -keyout evil_intermediate.key -out evil_intermediate.csr -noenc -subj "/CN=Intermediate CA"
openssl x509 -req -in evil_intermediate.csr -CA evil_root.pem -CAkey evil_root.key -CAcreateserial -out evil_intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")


openssl verify -x509_strict -CAfile root.pem intermediate.pem #check intermediate against root (issuer) - ok
openssl verify -x509_strict -CAfile intermediate.pem leaf.pem #intermediate is not CA certificate! root needs to be in trusted store, otherwise this will fail (err2)
openssl verify -x509_strict -untrusted intermediate.pem leaf.pem #root need to be in trusted store, otherwise this will fail (err20)
openssl verify -x509_strict -CAfile root.pem -untrusted intermediate.pem leaf.pem #check whole chain - ok

openssl verify -x509_strict -CAfile root.pem -untrusted evil_root.pem evil_intermediate.pem #fails, as it should
openssl verify -x509_strict -CAfile <(cat root.pem intermediate.pem) leaf.pem #passes, but what is tested against what? leaf against intermediate?
openssl verify -x509_strict -CAfile <(cat root.pem evil_root.pem) evil_intermediate.pem #passes, user passed self signed root as intermediate and evil_root-signed certificate as a leaf
openssl verify -x509_strict -untrusted evil_root.pem evil_intermediate.pem #fails, self-signed certificate in certificate chain
