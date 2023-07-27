package io.jsight;

public class ErrorPosition {
    private String filePath;
    private Integer line;
    private Integer col;
    private Integer index;

    public ErrorPosition(String filePath, Integer line, Integer col, Integer index) {
        this.filePath = filePath;
        this.line = line;
        this.col = col;
        this.index = index;
    }

    public String getFilepath() {
        return this.filePath;
    }

    public Integer getLine() {
        return this.line;
    }

    public Integer getCol() {
        return this.col;
    }

    public Integer getIndex() {
        return this.index;
    }
}