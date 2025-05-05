# Server Test Cases

## Multiple Messages with Delayed Newlines

### Test Command

```bash
(printf "hello\nworld"; sleep 2; printf "\n\n\nhallo\n") | nc localhost 4242
```

### Expected Output

```
client X: hello
client X: world
client X:
client X:
client X: hallo
```

### What This Tests

1. Immediate message handling:
   - "hello\n" is broadcast immediately
2. Message buffering:
   - "world" stays in buffer until newline
3. Multiple newlines:
   - Empty messages are broadcast for each newline
4. Delayed messages:
   - 2-second delay between messages
5. Final message:
   - "hallo\n" is broadcast with newline

### Why This is Important

- Verifies correct message buffering
- Tests handling of multiple newlines
- Confirms proper message broadcasting
- Validates timing and connection handling
