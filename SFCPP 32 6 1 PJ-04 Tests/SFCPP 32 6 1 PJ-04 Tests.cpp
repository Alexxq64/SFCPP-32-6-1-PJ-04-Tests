#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include the namespaces for GTest and GMock
using ::testing::Return;
using ::testing::InSequence;

// Define the IfcDBConnection interface
class IfcDBConnection {
public:
    virtual ~IfcDBConnection() {}

    virtual void open() = 0;
    virtual void close() = 0;
    virtual int execQuery(const std::string& query) = 0;
};

// Create the DBConnection class that implements the interface
class DBConnection : public IfcDBConnection {
public:
    void open() override {
    }

    void close() override {
    }

    int execQuery(const std::string& query) override {
        return 0; // Return value dummy
    }
};

// Create a mock object for the IfcDBConnection interface
class MockDBConnection : public IfcDBConnection {
public:
    MOCK_METHOD(void, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(int, execQuery, (const std::string& query), (override));
};

class ClassThatUsesDB {
public:
    ClassThatUsesDB(IfcDBConnection* dbConnection) : dbConnection_(dbConnection) {}

    void openConnection() {
        dbConnection_->open();
    }

    int useConnection(const std::string& query) {
        return dbConnection_->execQuery(query);
    }

    void closeConnection() {
        dbConnection_->close();
    }

private:
    IfcDBConnection* dbConnection_;
};

TEST(ClassThatUsesDBTest, TestOpenConnection) {
    // Create a mock object
    MockDBConnection mockDBConnection;

    // Set expectations for the mock object
    EXPECT_CALL(mockDBConnection, open()).Times(1);

    // Create an object of the class that uses the interface
    ClassThatUsesDB classThatUsesDB(&mockDBConnection);

    // Call a method that uses the IfcDBConnection interface
    classThatUsesDB.openConnection();
}

TEST(ClassThatUsesDBTest, TestUseConnection) {
    MockDBConnection mockDBConnection;

    const std::string query = "SELECT * FROM table";
    EXPECT_CALL(mockDBConnection, execQuery(query)).Times(1);

    ClassThatUsesDB classThatUsesDB(&mockDBConnection);

    classThatUsesDB.useConnection(query);
}

TEST(ClassThatUsesDBTest, TestCloseConnection) {
    MockDBConnection mockDBConnection;

    EXPECT_CALL(mockDBConnection, close()).Times(1);

    ClassThatUsesDB classThatUsesDB(&mockDBConnection);

    classThatUsesDB.closeConnection();
}

TEST(ClassThatUsesDBTest, TestUseConnectionReturnValue) {
    MockDBConnection mockDBConnection;

    const std::string query = "SELECT * FROM table";
    EXPECT_CALL(mockDBConnection, execQuery(query)).WillOnce(Return(42));

    ClassThatUsesDB classThatUsesDB(&mockDBConnection);

    int result = classThatUsesDB.useConnection(query);

    EXPECT_EQ(result, 42);
}

TEST(ClassThatUsesDBTest, TestOpenAndCloseConnectionSequence) {
    MockDBConnection mockDBConnection;

    InSequence seq;
    EXPECT_CALL(mockDBConnection, open()).Times(1);
    EXPECT_CALL(mockDBConnection, close()).Times(1);

    ClassThatUsesDB classThatUsesDB(&mockDBConnection);
    classThatUsesDB.openConnection();
    classThatUsesDB.closeConnection();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
