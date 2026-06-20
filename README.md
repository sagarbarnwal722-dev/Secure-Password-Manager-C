# Secure Password Manager in C

A password manager developed in C with secure credential storage and master password authentication.

## Features

* Master Password Authentication
* Add Password
* View Password
* Search Password
* Delete Password
* Password Encryption
* Activity Logging
* Automatic Backup

## Technologies Used

* C Programming
* File Handling
* Structures
* Basic Cryptography Concepts

## Compilation

gcc PasswordManager.c -o PasswordManager

## Run

./PasswordManager

## Screenshots

Screenshots demonstrating all features are available in the screenshots folder.

## Screenshots

### Master Password Login
![Master Login](password_manager/screenshots/enter_master_password.png)

### Add Password
![Add Password](password_manager/screenshots/add_password.png)

### View Password
![View Password](password_manager/screenshots/view_password.png)

### Search Password
![Search Password](password_manager/screenshots/search_password.png)

### Update Password
![Update Password](password_manager/screenshots/update_password.png)

### Delete Password
![Delete Password](password_manager/screenshots/delete_password.png)

### Activity Log
![Activity Log](password_manager/screenshots/activity_log.png)

### Password Encryption
![Password Encryption](password_manager/screenshots/encrypt_password.png)

### Master Password Encryption
![Master Password Encryption](password_manager/screenshots/encrypt_master.png)

### Backup Encryption
![Backup Encryption](password_manager/screenshots/backup_encrypt.png)

## How to Compile

```bash
gcc PasswordManager.c -o PasswordManager
```

## How to Run

```bash
PasswordManager.exe
```

## Project Structure

Secure-Password-Manager-C
│
├── password_manager
│   ├── PasswordManager.c
│   └── screenshots
│
├── README.md
└── .gitignore

## Requirements

- GCC Compiler
- Windows OS
- Standard C Libraries

## Future Enhancements

- AES-256 Encryption
- Cloud Backup
- Multi-User Support
- Password Strength Analytics
- Two-Factor Authentication (2FA)
