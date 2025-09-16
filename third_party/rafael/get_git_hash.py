import subprocess

def get_git_revision_hash():
    try:
        # Execute the 'git rev-parse HEAD' command
        full_hash = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD'])
        # Decode the byte string to a UTF-8 string and remove leading/trailing whitespace
        full_hash = full_hash.decode("utf-8").strip()
        return full_hash
    except subprocess.CalledProcessError:
        # Handle cases where Git is not found or not in a Git repository
        return None

# Example usage:
current_hash = get_git_revision_hash()
if current_hash:
    print(current_hash)
else:
    print("N/A")