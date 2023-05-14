#add certificate to the database
openssl ca -config crl.cnf -revoke intermediate.pem

#generate crl file based on the entries in the database
openssl ca -config crl.cnf -gencrl -out crl.pem
