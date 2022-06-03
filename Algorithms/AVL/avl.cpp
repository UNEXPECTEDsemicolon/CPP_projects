#include <iostream>

struct Node {
  int height;
  long value;
  Node *left;
  Node *right;
};

Node *NewNode(long value) {
  Node *node = new Node;
  node->height = 1;
  node->value = value;
  node->left = nullptr;
  node->right = nullptr;
  return node;
}

void DeleteTree(Node *node) {
  if (node != nullptr) {
    DeleteTree(node->right);
    DeleteTree(node->left);
    delete node;
  }
}

int Height(Node *node) {
  if (node == nullptr) {
    return 0;
  }
  return node->height;
}

int BalanceFactor(Node *node) {
  if (node->right != nullptr and node->left != nullptr) {
    return Height(node->right) - Height(node->left);
  }
  return 1;
}

void FixHeight(Node *node) {
  if (Height(node->left) > Height(node->right)) {
    node->height = Height(node->left) + 1;
  } else {
    node->height = Height(node->right) + 1;
  }
}

Node *RotateRight(Node *node_top) {
  Node *new_node_top = node_top->left;
  node_top->left = new_node_top->right;
  new_node_top->right = node_top;
  FixHeight(node_top);
  FixHeight(new_node_top);
  return new_node_top;
}

Node *RotateLeft(Node *node_top) {
  Node *new_node_top = node_top->right;
  node_top->right = new_node_top->left;
  new_node_top->left = node_top;
  FixHeight(node_top);
  FixHeight(new_node_top);
  return new_node_top;
}

Node *Balance(Node *node) {
  FixHeight(node);
  int b_factor = BalanceFactor(node);
  if (b_factor == 2) {
    if (BalanceFactor(node->right) < 0) {
      node->right = RotateRight(node->right);
    }
    return RotateLeft(node);
  }
  if (b_factor == -2) {
    if (BalanceFactor(node->left) > 0) {
      node->left = RotateLeft(node->left);
    }
    return RotateRight(node);
  }
  return node;
}

Node *Insert(Node *node, long value) {
  if (node == nullptr) {
    return NewNode(value);
  }
  if (value < node->value) {
    node->left = Insert(node->left, value);
  } else if (value > node->value) {
    node->right = Insert(node->right, value);
  } else {
    return node;
  }
  return Balance(node);
}

void FindNext(Node *node, int value, int &last_next) {
  int next = 0;
  if (node == nullptr) {
    std::cout << -1 << '\n';
    last_next = -1;
    return;
  }
  if (node != nullptr and node->value == value) {
    next = value;
    std::cout << next << '\n';
    last_next = next;
    return;
  }
  if (node != nullptr and node->value < value) {
    if (node->right == nullptr) {
      std::cout << -1 << '\n';
      last_next = -1;
      return;
    }
    FindNext(node->right, value, last_next);
  }
  if (node != nullptr and node->value > value) {
    if (node->left == nullptr) {
      std::cout << node->value << '\n';
      last_next = node->value;
      return;
    }
    next = node->value;
    node = node->left;
    if (node->value < value) {
      if (node->right == nullptr) {
        last_next = next;
        std::cout << next << '\n';
        return;
      }
      node = node->right;
      FindNext(node, value, last_next);
    }
    FindNext(node, value, last_next);
  }
}

int main() {
  int n;
  std::cin >> n;
  Node *avl_tree = nullptr;
  char last_request = '+';
  int last_next = 0;
  for (int i = 0; i < n; i++) {
    char request;
    std::cin >> request;
    int value;
    std::cin >> value;
    if (request == '+' and last_request == '+') {
      avl_tree = Insert(avl_tree, value);
    } else if (request == '+' and last_request == '?') {
      long new_value = (last_next + value) % 1000000000;
      avl_tree = Insert(avl_tree, new_value);
    } else {
      FindNext(avl_tree, value, last_next);
    }
    last_request = request;
  }
  DeleteTree(avl_tree);
  return 0;
}
