#missing flag
openssl verify -x509_strict -show_chain -CAfile root.pem -CRLfile crl.pem intermediate.pem

#verify with crl check
openssl verify -x509_strict -show_chain -crl_check -CAfile root.pem -CRLfile crl.pem intermediate.pem

