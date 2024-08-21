#include <gtest/gtest.h>
#include "latexmarkdown.h"


astnode* createSampleAST() {
    // Create nodes
    astnode *root = createNode("preamble", "", 0);
    astnode *child1 = createNode("subsection", "Background", 1);
    astnode *child2 = createNode("bold_text", "Important", 2);

    // Build the tree
    addChild(root, child1);
    addChild(child1, child2);

    return root;
}

// Test for createNode function
TEST(LatexMarkdownTest, CreateNode) {
    astnode *node = createNode("test_token", "test_data", 1);
    ASSERT_NE(node, nullptr);
    EXPECT_STREQ(node->token, "test_token");
    EXPECT_STREQ(node->data, "test_data");
    EXPECT_EQ(node->tabs, 1);
    EXPECT_EQ(node->child_count, 0);
    freeAST(node);
}

// Test for addChild function
TEST(LatexMarkdownTest, AddChild) {
    astnode *parent = createNode("parent", "parent_data", 0);
    astnode *child = createNode("child", "child_data", 1);
    addChild(parent, child);
    EXPECT_EQ(parent->child_count, 1);
    EXPECT_EQ(parent->children[0], child);
    freeAST(parent);
}

// Test for shouldSkip function
TEST(LatexMarkdownTest, ShouldSkip) {
    EXPECT_EQ(shouldSkip("sections_list"), 1);
    EXPECT_EQ(shouldSkip("unknown_token"), 0);
}

// Test for printAST function
TEST(LatexMarkdownTest, PrintAST) {
    astnode *root = createSampleAST();
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    printAST(root, file);
    fclose(file);
    freeAST(root);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "preamble: \n");
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "  subsection: Background\n");
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "    bold_text: Important\n");
    
    fclose(file);
}

// Test for handle_graphics function
TEST(LatexMarkdownTest, HandleGraphics) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    handle_graphics("{IIT Delhi Campus}", file);
    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "IIT Delhi Campus");

    fclose(file);
}

// Test for handle_href function
TEST(LatexMarkdownTest, HandleHref) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    handle_href("Here is a link: \\href{https://www.example.com}{Example}", file);
    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "Here is a link: [Example](https://www.example.com)");

    fclose(file);
}

// Test for handle_para function
TEST(LatexMarkdownTest, HandlePara) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    handle_para("This is a paragraph And another one.\\par", file);
    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "This is a paragraph And another one.\n");

    fclose(file);
}

// Test for process_item function
TEST(LatexMarkdownTest, ProcessItem) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    process_item("\\item This is an item.", file);
    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "This is an item.");

    fclose(file);
}

// Test for enumerate_list function
TEST(LatexMarkdownTest, EnumerateList) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    enumerate_list(1, file);
    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "1.");

    fclose(file);
}

// Test for count_column function
TEST(LatexMarkdownTest, CountColumn) {
    count_column("Column1 & Column2 & Column3");
    EXPECT_EQ(num_of_cols, 3);
}

TEST(LatexMarkdownTest, ProcessTableRow) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);

    char input_str[] = "Department & Programs \\\\";
    process_table_row(input_str, file);

    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[1002];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "|Department                                       | Programs                                        |");

    fclose(file);
}

// Test for add_table_separator function
TEST(LatexMarkdownTest, AddTableSeparator) {
    FILE *file = fopen("test_output.txt", "w");
    ASSERT_NE(file, nullptr);
    add_table_separator(3, file);
    fclose(file);

    file = fopen("test_output.txt", "r");
    ASSERT_NE(file, nullptr);

    char line[200];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "|-------------------------------------------------|-------------------------------------------------|-------------------------------------------------|\n");

    fclose(file);
}

// Test for createMarkdown function
TEST(LatexMarkdownTest, CreateMarkdown) {
    astnode *root = createSampleAST();
    FILE *file = fopen("test_output.md", "w");
    ASSERT_NE(file, nullptr);
    createMarkdown(root, file);
    fclose(file);
    freeAST(root);

    file = fopen("test_output.md", "r");
    ASSERT_NE(file, nullptr);

    char line[100];
    fgets(line, sizeof(line), file);
    EXPECT_STREQ(line, "## Background**Important**");

    fclose(file);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

