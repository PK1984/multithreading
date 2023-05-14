#the below examples contain examples of wrong usage as well

openssl verify -x509_strict -CAfile root.pem intermediate.pem #check intermediate against root (issuer) - ok
openssl verify -x509_strict -CAfile intermediate.pem leaf.pem #intermediate is not CA certificate! root needs to be in trusted store, otherwise this will fail (err2)
openssl verify -x509_strict -untrusted intermediate.pem leaf.pem #root need to be in trusted store, otherwise this will fail (err20)
openssl verify -x509_strict -CAfile root.pem -untrusted intermediate.pem leaf.pem #check whole chain - ok
openssl verify -show_chain -partial_chain -x509_strict -trusted intermediate.pem leaf.pem 

openssl verify -x509_strict -CAfile root.pem -untrusted evil_root.pem evil_intermediate.pem #fails, as it should
openssl verify -x509_strict -CAfile <(cat root.pem intermediate.pem) leaf.pem #passes, but what is tested against what? leaf against intermediate?
openssl verify -x509_strict -CAfile <(cat root.pem evil_root.pem) evil_intermediate.pem #passes, user passed self signed root as intermediate and evil_root-signed certificate as a leaf
openssl verify -x509_strict -untrusted evil_root.pem evil_intermediate.pem #fails, self-signed certificate in certificate chain
