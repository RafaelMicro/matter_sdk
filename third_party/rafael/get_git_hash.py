import subprocess

def get_git_revision_hash():
    try:
        # Execute the 'git rev-parse HEAD' command
        full_hash = subprocess.check_output(['git', 'rev-parse', 'HEAD'])
        # Take the first 8 hex chars to fit in uint32_t (printf("%.8x", BUILD_HASH_INFO))
        full_hash = full_hash.decode("utf-8").strip()[:8]
        return full_hash
    except subprocess.CalledProcessError:
        # Handle cases where Git is not found or not in a Git repository
        return None

# Example usage:
current_hash = get_git_revision_hash()
if current_hash:
    print(current_hash)
else:
    print("00000000")