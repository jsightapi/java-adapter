package io.jsight;

public class ErrorPosition {
    private String filePath;
    private int line;
    private int col;
    private int index;

    public ErrorPosition(String filePath, int line, int col, int index) {
        this.filePath = filePath;
        this.line = line;
        this.col = col;
        this.index = index;
    }

    public String getFilepath() {
        return this.filePath;
    }

    public int getLine() {
        return this.line;
    }

    public int getCol() {
        return this.col;
    }

    public int getIndex() {
        return this.index;
    }
}