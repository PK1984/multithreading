openssl verify -show_chain -CAfile root.pem -untrusted intermediate.pem leaf.pem
