# Q10. What is a .gitignore file?

A **.gitignore** file tells Git which files and folders should not be tracked or uploaded to the repository. It is commonly used to ignore compiled files, temporary files, operating system files, and IDE configuration folders that are not required in the project.

## .gitignore for an Arduino Project

```gitignore
# Compiled output files
*.hex
*.elf

# OS-specific files
.DS_Store
Thumbs.db

# IDE configuration folders
.vscode/
build/
