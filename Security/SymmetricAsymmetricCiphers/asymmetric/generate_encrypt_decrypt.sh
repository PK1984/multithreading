openssl genpkey -algorithm RSA -out private_key.pem -pkeyopt rsa_keygen_bits:2048
openssl pkey -in private_key.pem -out public_key.pem -pubout

openssl pkey -inform PEM -in public_key.pem -outform DER -out public_key.der -pubin

openssl pkey -inform PEM -in private_key.pem -outform DER -out public_key2.der -pubout

cmp public_key.der public_key2.der

openssl pkey -in private_key.pem -text_pub

openssl pkeyutl -encrypt -inkey public_key.pem -pubin -in plain_text.txt -out cipher_text

openssl pkeyutl -decrypt -inkey private_key.pem -in cipher_text
