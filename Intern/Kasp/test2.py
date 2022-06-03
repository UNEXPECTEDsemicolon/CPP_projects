s = input()

br = {"(": ")", "[": "]", "<": ">", "{": "}"}
inv_br = {value: key for (key, value) in br.items()}

stack = []
last_brace_pos = -1
for i in range(len(s)):
    if s[i] in br:
        last_brace_pos = i
        stack.append(s[i])
    elif s[i] in inv_br:
        last_brace_pos = i
        if len(stack) == 0 or stack.pop() != inv_br[s[i]]:
            print(i + 1)
            break
else:
    print(last_brace_pos + 1 if len(stack) else "True")