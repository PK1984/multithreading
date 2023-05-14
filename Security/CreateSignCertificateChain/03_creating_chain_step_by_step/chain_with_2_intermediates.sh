#create self signed root certificate
openssl req -x509 -newkey rsa:2048 -keyout root.key -out root.pem -days 365 -noenc -subj "/CN=Root CA"  --addext "keyUsage=critical,keyCertSign,cRLSign"

#create key and intermediate certificate signing requets
openssl req -new -newkey rsa:2048 -keyout intermediate.key -out intermediate.csr -noenc -subj "/CN=Intermediate CA"

#inspect the crs
openssl req -in  intermediate.csr -noout -text

#sign intermediate certificate by the root
openssl x509 -req -in intermediate.csr -CA root.pem -CAkey root.key -CAcreateserial -out intermediate.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")

#create another key and intermediate certificate signing requets
openssl req -new -newkey rsa:2048 -keyout intermediate2.key -out intermediate2.csr -noenc -subj "/CN=Intermediate CA"
#sign certificate intermediate2 by intermediate
openssl x509 -req -in intermediate2.csr -CA intermediate.pem -CAkey intermediate.key -CAcreateserial -out intermediate2.pem -days 365 -extfile <(printf "basicConstraints=critical,CA:true,pathlen:0\nkeyUsage=critical,keyCertSign,cRLSign\n")

#create end entity (leaf) certificate
openssl req -new -newkey rsa:2048 -keyout leaf.key -out leaf.csr -noenc -subj "/CN=Leaf Certificate"
#sign leaf certificate by intermediate2
openssl x509 -req -in leaf.csr -CA intermediate2.pem -CAkey intermediate2.key -CAcreateserial -out leaf.pem -days 365 -extfile <(printf "authorityKeyIdentifier=keyid,issuer\nsubjectKeyIdentifier=hash\nkeyUsage=critical,digitalSignature,keyEncipherment\nbasicConstraints=critical,CA:false")

#concatenate intermediates into single file (first the signee, then signer)
cat intermediate2.pem intermediate.pem > intermediates.pem
echo "###check intermediate against root (issuer)###" 
openssl verify -x509_strict -CAfile root.pem intermediate.pem #check intermediate against root (issuer) - ok
echo "###check intermediate2 against intermediate###"
openssl verify -x509_strict -CAfile intermediate.pem intermediate2.pem #check intermediate2 against intermediate2 (issuer) - nok
echo "###check leaf against intermediate2###"
openssl verify -x509_strict -CAfile intermediate2.pem leaf.pem #check leaf against intermediate2 (issuer) - nok
echo "###check whole chain###"
openssl verify -x509_strict -show_chain -CAfile root.pem -untrusted intermediates.pem leaf.pem #check whole chain - ok
