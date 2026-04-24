#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// BLOCK 0 - Tree Node Structures
// ============================================================================

struct TreeNode {
  int value;
  std::vector<std::shared_ptr<TreeNode>> children;
};

// Binary Search Tree Node
struct BSTNode {
  int value;
  std::shared_ptr<BSTNode> left;
  std::shared_ptr<BSTNode> right;
};

// Expression Tree Node
struct ExprNode {
  char op; // operator: +, -, *, /, ^, or 'N' for number/variable
  double value;
  std::string variable;
  std::shared_ptr<ExprNode> left;
  std::shared_ptr<ExprNode> right;
};

// Type alias for paths
using Path = std::vector<int>;

// BLOCK 1 - N-ary Tree Functions
// ============================================================================

// Create new tree node
std::shared_ptr<TreeNode> createNode(int value) {
  auto node = std::make_shared<TreeNode>();
  node->value = value;
  return node;
}

// Get path to element (DFS)
bool getPathToElement(std::shared_ptr<TreeNode> node, int value, Path &path) {
  if (!node)
    return false;

  if (node->value == value) {
    return true;
  }

  for (size_t i = 0; i < node->children.size(); i++) {
    path.push_back(i);
    if (getPathToElement(node->children[i], value, path)) {
      return true;
    }
    path.pop_back();
  }

  return false;
}

// Get element by path
std::shared_ptr<TreeNode> getElementByPath(std::shared_ptr<TreeNode> root,
                                           const Path &path) {
  std::shared_ptr<TreeNode> current = root;

  for (int index : path) {
    if (!current || index < 0 || index >= (int)current->children.size()) {
      return nullptr;
    }
    current = current->children[index];
  }

  return current;
}

// Add child at given path
bool addChildAtPath(std::shared_ptr<TreeNode> root, const Path &path,
                    int value) {
  auto parent = getElementByPath(root, path);
  if (!parent)
    return false;

  parent->children.push_back(createNode(value));
  return true;
}

// BLOCK 2 - N-ary Tree Output Functions
// ========================================================================

void printWithBrackets(std::shared_ptr<TreeNode> node) {
  if (!node)
    return;

  std::cout << node->value;
  if (!node->children.empty()) {
    std::cout << "(";
    for (size_t i = 0; i < node->children.size(); i++) {
      if (i > 0)
        std::cout << " ";
      printWithBrackets(node->children[i]);
    }
    std::cout << ")";
  }
}

void printWithIndentation(std::shared_ptr<TreeNode> node, int indent = 0) {
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    std::cout << "  ";
  std::cout << node->value << std::endl;

  for (auto &child : node->children) {
    printWithIndentation(child, indent + 1);
  }
}

void printLevelOrder(std::shared_ptr<TreeNode> root) {
  if (!root)
    return;

  std::vector<std::shared_ptr<TreeNode>> current, next;
  current.push_back(root);

  int level = 0;
  while (!current.empty()) {
    std::cout << "Level " << level << ": ";
    for (auto node : current) {
      std::cout << node->value << " ";
    }
    std::cout << "\n";

    for (auto node : current) {
      for (auto child : node->children) {
        next.push_back(child);
      }
    }

    current = next;
    next.clear();
    level++;
  }
}

// BLOCK 3 - N-ary Tree Deletion Functions
// ========================================================================

std::shared_ptr<TreeNode> deleteByPath(std::shared_ptr<TreeNode> root,
                                       const Path &path) {
  if (path.empty())
    return nullptr; // Cannot delete root

  Path parentPath(path.begin(), path.end() - 1);
  int childIndex = path.back();

  auto parent = getElementByPath(root, parentPath);
  if (!parent || childIndex < 0 || childIndex >= (int)parent->children.size()) {
    return nullptr;
  }

  auto deleted = parent->children[childIndex];
  parent->children.erase(parent->children.begin() + childIndex);
  return deleted;
}

bool deleteFirstByValue(std::shared_ptr<TreeNode> root, int value) {
  Path path;
  if (getPathToElement(root, value, path)) {
    if (path.empty())
      return false; // Cannot delete root
    deleteByPath(root, path);
    return true;
  }
  return false;
}

int deleteAllByValueHelper(std::shared_ptr<TreeNode> node, int value) {
  if (!node)
    return 0;

  int count = 0;
  for (auto it = node->children.begin(); it != node->children.end();) {
    if ((*it)->value == value) {
      it = node->children.erase(it);
      count++;
    } else {
      count += deleteAllByValueHelper(*it, value);
      ++it;
    }
  }
  return count;
}

int deleteAllByValue(std::shared_ptr<TreeNode> root, int value) {
  return deleteAllByValueHelper(root, value);
}

void printPathToElement(std::shared_ptr<TreeNode> root, int value) {
  Path path;
  if (getPathToElement(root, value, path)) {
    std::cout << "Path to " << value << ": ";
    for (int i : path) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "Element " << value << " not found" << std::endl;
  }
}

// BLOCK 4 - Binary Search Tree Functions
// ============================================================================

std::shared_ptr<BSTNode> createBSTNode(int value) {
  auto node = std::make_shared<BSTNode>();
  node->value = value;
  node->left = nullptr;
  node->right = nullptr;
  return node;
}

void bstInsert(std::shared_ptr<BSTNode> &node, int value) {
  if (!node) {
    node = createBSTNode(value);
  } else if (value < node->value) {
    bstInsert(node->left, value);
  } else {
    bstInsert(node->right, value);
  }
}

void printBSTWithIndentation(std::shared_ptr<BSTNode> node, int indent = 0) {
  if (!node)
    return;

  for (int i = 0; i < indent; i++)
    std::cout << "  ";
  std::cout << node->value << std::endl;

  if (node->left || node->right) {
    if (node->left) {
      printBSTWithIndentation(node->left, indent + 1);
    }
    if (node->right) {
      printBSTWithIndentation(node->right, indent + 1);
    }
  }
}

void printBSTLevelOrder(std::shared_ptr<BSTNode> root) {
  if (!root)
    return;

  std::vector<std::shared_ptr<BSTNode>> current, next;
  current.push_back(root);

  int level = 0;
  while (!current.empty()) {
    std::cout << "Level " << level << ": ";
    for (auto node : current) {
      std::cout << node->value << " ";
    }
    std::cout << std::endl;

    for (auto node : current) {
      if (node->left)
        next.push_back(node->left);
      if (node->right)
        next.push_back(node->right);
    }

    current = next;
    next.clear();
    level++;
  }
}

// BLOCK 6 - Expression Tree Functions
// ============================================================================

std::shared_ptr<ExprNode> createExprNode() {
  auto node = std::make_shared<ExprNode>();
  node->op = 'N';
  node->value = 0;
  node->left = nullptr;
  node->right = nullptr;
  return node;
}

std::string removeSpaces(const std::string &s) {
  std::string result;
  for (char c : s) {
    if (c != ' ')
      result += c;
  }
  return result;
}

std::shared_ptr<ExprNode> parseExprHelper(const std::string &expr, int start,
                                          int end);

std::shared_ptr<ExprNode> parseExprHelper(const std::string &expr, int start,
                                          int end) {
  if (start > end)
    return nullptr;

  // Find operator with lowest precedence
  int level = 0;
  int opPos = -1;
  char op = '\0';

  // Look for + or -
  for (int i = end; i >= start; i--) {
    if (expr[i] == '(')
      level++;
    else if (expr[i] == ')')
      level--;
    else if (level == 0 && (expr[i] == '+' || expr[i] == '-')) {
      opPos = i;
      op = expr[i];
      break;
    }
  }

  // Look for * or /
  if (opPos == -1) {
    level = 0;
    for (int i = end; i >= start; i--) {
      if (expr[i] == '(')
        level++;
      else if (expr[i] == ')')
        level--;
      else if (level == 0 && (expr[i] == '*' || expr[i] == '/')) {
        opPos = i;
        op = expr[i];
        break;
      }
    }
  }

  // Look for ^
  if (opPos == -1) {
    level = 0;
    for (int i = end; i >= start; i--) {
      if (expr[i] == '(')
        level++;
      else if (expr[i] == ')')
        level--;
      else if (level == 0 && expr[i] == '^') {
        opPos = i;
        op = expr[i];
        break;
      }
    }
  }

  if (opPos != -1) {
    auto node = createExprNode();
    node->op = op;
    node->left = parseExprHelper(expr, start, opPos - 1);
    node->right = parseExprHelper(expr, opPos + 1, end);
    return node;
  }

  // Remove parentheses
  if (expr[start] == '(' && expr[end] == ')') {
    return parseExprHelper(expr, start + 1, end - 1);
  }

  // It's a number or variable
  auto node = createExprNode();
  node->op = 'N';

  if (std::isalpha(expr[start])) {
    node->variable = expr.substr(start, end - start + 1);
  } else {
    node->value = std::stod(expr.substr(start, end - start + 1));
  }

  return node;
}

// Parse expression
std::shared_ptr<ExprNode> parseExpression(const std::string &expr) {
  std::string cleaned = removeSpaces(expr);
  return parseExprHelper(cleaned, 0, cleaned.length() - 1);
}

// Evaluate expression helper
double
evaluateExprHelper(std::shared_ptr<ExprNode> node,
                   const std::vector<std::pair<std::string, double>> &vars) {
  if (!node)
    return 0;

  if (node->op == 'N') {
    if (!node->variable.empty()) {
      for (const auto &var : vars) {
        if (var.first == node->variable) {
          return var.second;
        }
      }
      return 0; // Variable not found
    }
    return node->value;
  }

  double left = evaluateExprHelper(node->left, vars);
  double right = evaluateExprHelper(node->right, vars);

  switch (node->op) {
  case '+':
    return left + right;
  case '-':
    return left - right;
  case '*':
    return left * right;
  case '/':
    if (right == 0) {
      std::cout << "Error: Division by zero!" << std::endl;
      return 0;
    }
    return left / right;
  case '^':
    return std::pow(left, right);
  default:
    return 0;
  }
}

// Evaluate expression
double evaluateExpression(
    std::shared_ptr<ExprNode> root,
    const std::vector<std::pair<std::string, double>> &vars = {}) {
  return evaluateExprHelper(root, vars);
}

void printExprHelper(std::shared_ptr<ExprNode> node) {
  if (!node)
    return;

  if (node->op == 'N') {
    if (!node->variable.empty()) {
      std::cout << node->variable;
    } else {
      std::cout << node->value;
    }
  } else {
    std::cout << "(";
    printExprHelper(node->left);
    std::cout << " " << node->op << " ";
    printExprHelper(node->right);
    std::cout << ")";
  }
}

void printExpression(std::shared_ptr<ExprNode> node) {
  printExprHelper(node);
  std::cout << std::endl;
}

// DEMONSTRATION MODE
// ============================================================================

void demonstrationMode() {
  std::cout << std::endl;
  std::cout << "=== DEMONSTRATION MODE ===" << std::endl << std::endl;

  // Demonstrate N-ary Tree
  std::cout << "--- N-ary Tree Demonstration ---" << std::endl;
  auto root = createNode(1);
  addChildAtPath(root, {}, 2);
  addChildAtPath(root, {}, 3);
  addChildAtPath(root, {0}, 4);
  addChildAtPath(root, {0}, 5);
  addChildAtPath(root, {1}, 6);
  addChildAtPath(root, {0, 0}, 7);
  std::cout << std::endl;
  std::cout << "1. Tree with brackets:" << std::endl;
  printWithBrackets(root);
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "2. Tree with indentation:" << std::endl;
  printWithIndentation(root);
  std::cout << std::endl;
  std::cout << "3. Level-order output:" << std::endl;
  printLevelOrder(root);

  std::cout << std::endl;
  std::cout << "Path to element 7: ";
  printPathToElement(root, 7);

  // Demonstrate BST
  std::cout << std::endl;
  std::cout << "--- Binary Search Tree Demonstration ---" << std::endl;
  std::shared_ptr<BSTNode> bst = nullptr;
  int values[] = {5, 3, 7, 1, 4, 6, 9};
  for (int v : values) {
    bstInsert(bst, v);
  }
  std::cout << std::endl;
  std::cout << "BST with indentation:" << std::endl;
  printBSTWithIndentation(bst);
  std::cout << std::endl;
  std::cout << "BST level-order:" << std::endl;
  printBSTLevelOrder(bst);

  // Demonstrate Expression Tree
  std::cout << std::endl;
  std::cout << "--- Expression Tree Demonstration ---" << std::endl;
  auto expr = parseExpression("2+3*4");
  std::cout << "Expression: ";
  printExpression(expr);
  std::cout << "Result: " << evaluateExpression(expr) << std::endl;

  auto expr2 = parseExpression("10/(2+3)");
  std::cout << "Expression: ";
  printExpression(expr2);
  std::cout << "Result: " << evaluateExpression(expr2) << std::endl;
}

// INTERACTIVE MODE
// ============================================================================

void interactiveMode() {
  std::cout << "\n=== INTERACTIVE MODE ===" << std::endl << std::endl;
  std::cout << "Choose data structure:" << std::endl;
  std::cout << "1. N-ary Tree" << std::endl;
  std::cout << "2. Binary Search Tree" << std::endl;
  std::cout << "3. Expression Tree" << std::endl;
  std::cout << "0. Exit" << std::endl;

  int choice;
  std::cin >> choice;

  if (choice == 1) {
    std::cout << std::endl;
    std::cout << "Enter root value: ";
    int rootVal;
    std::cin >> rootVal;
    auto tree = createNode(rootVal);

    int cmd;
    while (true) {
      std::cout << "\nN-ary Tree Commands:" << std::endl;
      std::cout << "1. Add child" << std::endl;
      std::cout << "2. Print with brackets" << std::endl;
      std::cout << "3. Print with indentation" << std::endl;
      std::cout << "4. Print level-order" << std::endl;
      std::cout << "5. Delete by value" << std::endl;
      std::cout << "0. Exit" << std::endl;
      std::cin >> cmd;

      if (cmd == 0)
        break;
      else if (cmd == 1) {
        std::cout << "Enter path (space-separated, empty line for root): ";
        std::cin.ignore();
        std::string line;
        std::getline(std::cin, line);
        Path path;
        if (!line.empty()) {
          std::istringstream iss(line);
          int idx;
          while (iss >> idx)
            path.push_back(idx);
        }
        std::cout << "Enter value: ";
        int val;
        std::cin >> val;
        if (addChildAtPath(tree, path, val)) {
          std::cout << "Added successfully" << std::endl;
        } else {
          std::cout << "Failed to add" << std::endl;
        }
      } else if (cmd == 2) {
        std::cout << "Tree: ";
        printWithBrackets(tree);
        std::cout << std::endl;
      } else if (cmd == 3) {
        printWithIndentation(tree);
      } else if (cmd == 4) {
        printLevelOrder(tree);
      } else if (cmd == 5) {
        std::cout << "Enter value to delete: ";
        int val;
        std::cin >> val;
        int count = deleteAllByValue(tree, val);
        std::cout << "Deleted " << count << " node(s)" << std::endl;
      }
    }
  } else if (choice == 2) {
    std::shared_ptr<BSTNode> bst = nullptr;
    int cmd;
    while (true) {
      std::cout << "\nBST Commands:" << std::endl;
      std::cout << "1. Insert value" << std::endl;
      std::cout << "2. Print with indentation" << std::endl;
      std::cout << "3. Print level-order" << std::endl;
      std::cout << "0. Exit" << std::endl;
      std::cin >> cmd;

      if (cmd == 0)
        break;
      else if (cmd == 1) {
        std::cout << "Enter value: ";
        int val;
        std::cin >> val;
        bstInsert(bst, val);
      } else if (cmd == 2) {
        printBSTWithIndentation(bst);
      } else if (cmd == 3) {
        printBSTLevelOrder(bst);
      }
    }
  } else if (choice == 3) {
    std::cout << "Enter expression (e.g., 2+3*4): ";
    std::cin.ignore();
    std::string exprStr;
    std::getline(std::cin, exprStr);
    auto expr = parseExpression(exprStr);

    std::cout << "Expression: ";
    printExpression(expr);
    std::cout << "Result: " << evaluateExpression(expr) << std::endl;
  }
}

// MAIN PROGRAM
// ============================================================================

int main() {
  std::cout << "========================================" << std::endl;
  std::cout << "    Lab 4 - Tree Data Structures" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Tasks: 2, 7, 8, 9, 13, 14, 15, 18, 22" << std::endl
            << std::endl;

  std::cout << "Choose mode:" << std::endl;
  std::cout << "1. Demonstration mode" << std::endl;
  std::cout << "2. Interactive mode" << std::endl;
  std::cout << "0. Exit" << std::endl;

  int mode;
  std::cin >> mode;

  if (mode == 1) {
    demonstrationMode();
  } else if (mode == 2) {
    interactiveMode();
  }

  return 0;
}
