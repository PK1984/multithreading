#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

std::string rsa_encrypt(EVP_PKEY* pubkey, const std::string& plaintext){
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pubkey, nullptr);
	if (ctx == nullptr) {
		std::cerr << "Error creating context for encryption" << std::endl;
		return "";
	}
	if (EVP_PKEY_encrypt_init(ctx) <= 0) {
		std::cerr << "Error initializing encryption" << std::endl;
		return "";
	}
	if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
		std::cerr << "Error setting RSA padding for encryption" << std::endl;
		return "";
	}
	const auto RSA_PKCS1_PADDING_size = 11; // for RSA_PKCS1_PADDING
	if (RSA_PKCS1_PADDING_size + plaintext.length() > 256) {
		std::cerr << "Block for encryption too large, divide the input string into smaller blocks" << std::endl;
		return "";
	}
	const unsigned char* plaintext_ucstr = reinterpret_cast<const unsigned char*>(plaintext.c_str());

	std::size_t encrypted_len = EVP_PKEY_size(pubkey);
	unsigned char* encrypted = new unsigned char[encrypted_len];

	if (EVP_PKEY_encrypt(ctx, encrypted, &encrypted_len, plaintext_ucstr, plaintext.length()) <= 0) {
		std::cerr << "Error encrypting message" << std::endl;
		return "";
	}
	EVP_PKEY_CTX_free(ctx);
	auto output = std::string(reinterpret_cast<const char*>(encrypted), encrypted_len);
	delete[] encrypted;
	return output;
}

std::string rsa_decrypt(EVP_PKEY* privkey, const std::string& ciphertext){
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(privkey, nullptr);
	if (ctx == nullptr) {
		std::cerr << "Error creating context for decryption" << std::endl;
		return "";
	} 
	if (EVP_PKEY_decrypt_init(ctx) <= 0) {
		std::cerr << "Error initializing decryption" << std::endl;
		return "";
	}
	if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
		std::cerr << "Error setting RSA padding for decryption" << std::endl;
		return "";
	}
	const unsigned char* ciphertext_ucstr = reinterpret_cast<const unsigned char*>(ciphertext.c_str());

	std::size_t decrypted_len{0u};
	if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, ciphertext_ucstr, ciphertext.length()) <= 0)
	{
		return "";
	}
	else
	{
		std::cout << "Length of decrypted message is: " << decrypted_len << std::endl;
	}
	unsigned char* decrypted = new unsigned char[decrypted_len];
	if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, ciphertext_ucstr, ciphertext.length()) <= 0) {
		std::cerr << "Error decrypting message" << std::endl;
		return "";
	}
	EVP_PKEY_CTX_free(ctx);
	auto output = std::string(reinterpret_cast<const char*>(decrypted), decrypted_len);
	delete[] decrypted;
	return output;
}

int main() {
	// Load the public key from a file
	FILE* pubkey_file = fopen("public_key.pem", "rb");
	EVP_PKEY* pubkey = PEM_read_PUBKEY(pubkey_file, nullptr, nullptr, nullptr);
	fclose(pubkey_file);

	// Load the private key from a file
	FILE* privkey_file = fopen("private_key.pem", "rb");
	EVP_PKEY* privkey = PEM_read_PrivateKey(privkey_file, nullptr, nullptr, nullptr);
	fclose(privkey_file);
	
	const std::string plaintxt{"Na ten czas Wojski chwycil na tasmie przypiety swoj roga bawoli, dlugi centokowany krety jak waz boa."};
	auto ciphertext = rsa_encrypt(pubkey, plaintxt);
	auto plaintext = rsa_decrypt(privkey, ciphertext);

	std::cout << "Decrypted message: " << plaintext << std::endl;

	EVP_PKEY_free(pubkey);
	EVP_PKEY_free(privkey);

	return 0;
}
