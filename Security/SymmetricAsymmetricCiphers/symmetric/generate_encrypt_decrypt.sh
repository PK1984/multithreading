openssl rand -hex 16 > iv.hex
openssl rand -hex 32 > 256b_key.hex
openssl rand -hex 128 > passphrase.txt
openssl enc -e -aes-256-cbc -in plain_text.txt -out plain_text.txt.enc -k $(cat passphrase.txt) -pbkdf2
openssl enc -e -aes-256-cbc -in plain_text.txt -out plain_text.txt.enc2 -k passphrase.txt -pbkdf2 # wrong usage!
openssl enc -e -aes-256-cbc -in plain_text.txt -out plain_text.txt.enc3 -kfile passphrase.txt -pbkdf2
#openssl enc -d -aes-256-cbc -in plain_text.txt.enc -k $(cat passphrase.txt) -pbkdf2

openssl enc -e -aes-256-cbc -in plain_text.txt -out plain_text.txt.enc4 -K $(cat 256b_key.hex) -iv $(cat iv.hex)
#openssl enc -d -aes-256-cbc -in plain_text.txt.enc2 -K $(cat 256b_key.hex) -iv $(cat iv.hex)


