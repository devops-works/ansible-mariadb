#!/bin/bash

# GPG Key Auto-Update for Backup Encryption
# Updates expired keys from specified URLs

set -e

# Configuration: Add your key mappings here
# Format: "KEY_ID|URL"
declare -a KEY_MAPPINGS=(
    #"0x1234567890ABCDEF|https://example.com/keys/user1.asc"
    #"0xFEDCBA0987654321|https://example.org/keys/user2.asc"
    # Add more mappings as needed
)

# Alternatively, load from config file
CONFIG_FILE="${GPG_UPDATE_CONFIG:-$HOME/.gpg-key-updates.conf}"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log_info() { echo -e "${GREEN}[INFO]${NC} $1"; }
log_warn() { echo -e "${YELLOW}[WARN]${NC} $1"; }
log_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# Load mappings from config file if it exists
load_config() {
    if [ -f "$CONFIG_FILE" ]; then
        log_info "Loading configuration from $CONFIG_FILE"
        while IFS='|' read -r keyid url || [ -n "$keyid" ]; do
            # Skip comments and empty lines
            [[ "$keyid" =~ ^#.*$ ]] && continue
            [[ -z "$keyid" ]] && continue
            KEY_MAPPINGS+=("$keyid|$url")
        done < "$CONFIG_FILE"
    fi
}

# Check if a key is expired
is_key_expired() {
    local keyid="$1"
    
    # Get key expiration info
    local exp_status=$(gpg --list-keys --with-colons "$keyid" 2>/dev/null | \
        awk -F: '/^pub:/ {print $2}')
    
    # Status codes: e=expired, r=revoked, -=no expiration
    if [ "$exp_status" = "e" ]; then
        return 0  # Key is expired
    else
        return 1  # Key is valid or doesn't exist
    fi
}

# Update a single key
update_key() {
    local keyid="$1"
    local url="$2"
    local temp_key=$(mktemp)
    local temp_log=$(mktemp)
    
    log_info "Updating key $keyid from $url"
    
    # Download the key
    if ! curl -sS -f -L -o "$temp_key" "$url"; then
        log_error "Failed to download key from $url"
        rm -f "$temp_key" "$temp_log"
        return 1
    fi
    
    # Import the key and capture output
    if gpg --import "$temp_key" 2>&1 | tee "$temp_log"; then
        # Check for successful import indicators
        # Success patterns: "imported:", "unchanged:", "new user IDs:", "new signatures:", "new subkeys:"
        if grep -qE "(imported:|unchanged:|new user IDs:|new signatures:|new subkeys:|Total number processed:)" "$temp_log"; then
            log_info "✓ Successfully updated key $keyid"
            rm -f "$temp_key" "$temp_log"
            return 0
        else
            log_error "Import completed but no changes detected for key $keyid"
            cat "$temp_log"
            rm -f "$temp_key" "$temp_log"
            return 1
        fi
    else
        log_error "GPG import command failed for key $keyid"
        cat "$temp_log"
        rm -f "$temp_key" "$temp_log"
        return 1
    fi
}

# Main logic
main() {
    load_config
    
    if [ ${#KEY_MAPPINGS[@]} -eq 0 ]; then
        log_error "No key mappings configured"
        log_info "Edit the script or create $CONFIG_FILE"
        exit 1
    fi
    
    log_info "Checking ${#KEY_MAPPINGS[@]} key(s) for expiration..."
    echo ""
    
    local updated=0
    local failed=0
    local skipped=0
    
    for mapping in "${KEY_MAPPINGS[@]}"; do
        IFS='|' read -r keyid url <<< "$mapping"
        
        # Trim whitespace
        keyid=$(echo "$keyid" | xargs)
        url=$(echo "$url" | xargs)
        
        log_info "Checking key: $keyid"
        
        if is_key_expired "$keyid"; then
            log_warn "Key $keyid is EXPIRED - updating..."
            if update_key "$keyid" "$url"; then
                updated=$((updated + 1))
            else
                failed=$((failed + 1))
            fi
        else
            # Check if key exists
            if gpg --list-keys "$keyid" &>/dev/null; then
                log_info "Key $keyid is still valid - skipping"
                skipped=$((skipped + 1))
            else
                log_warn "Key $keyid not found in keyring - importing..."
                if update_key "$keyid" "$url"; then
                    updated=$((updated + 1))
                else
                    failed=$((failed + 1))
                fi
            fi
        fi
        echo ""
    done
    
    log_info "========================================="
    log_info "Summary:"
    log_info "  Keys updated: $updated"
    log_info "  Keys skipped (valid): $skipped"
    log_info "  Failed: $failed"
    log_info "========================================="
    
    [ $failed -eq 0 ] && exit 0 || exit 1
}

main "$@"

