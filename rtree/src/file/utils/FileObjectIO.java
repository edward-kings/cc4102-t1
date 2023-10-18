package file.utils;

import java.io.*;

public class FileObjectIO<T> {
    private T dummy;
    public FileObjectIO(T sample) {
        dummy = sample;
    }
    private ObjectOutputStream outputStream;
    private RandomAccessFile file;
    public void setStreamToFile(RandomAccessFile file) {

    }
    public int calculateObjectSize() throws IOException {
        try (ByteArrayOutputStream byteStream = new ByteArrayOutputStream()) {
            try (ObjectOutputStream objectStream = new ObjectOutputStream(byteStream)) {

                objectStream.writeObject(dummy);
                objectStream.flush();

                return byteStream.toByteArray().length;
            }
        }
    }

    @SafeVarargs
    public final void writeObjectsToFile(FileOutputStream fileOutputStream, T... objects) throws IOException {
        ObjectOutputStream outputStream = new ObjectOutputStream(fileOutputStream);
        try {
            for (T obj: objects) {
                outputStream.writeObject(obj);
                outputStream.flush();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public T readObjectFromFile(RandomAccessFile randomAccessFile, int objectIndex) {
        try  {
            long offset = objectIndex;

            randomAccessFile.seek(offset);
            ObjectInputStream inputStream = new ObjectInputStream(new FileInputStream(randomAccessFile.getFD()));
            T res = (T) inputStream.readObject();
            inputStream.close();
            return res;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            System.out.println(e);
        }
        return null;
    }
    public T[] readObjectArrayFromFile(RandomAccessFile randomAccessFile, int objectIndex) {
        try  {
            long offset = objectIndex;

            randomAccessFile.seek(offset);
            ObjectInputStream inputStream = new ObjectInputStream(new FileInputStream(randomAccessFile.getFD()));
            return (T[]) inputStream.readObject();
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }
}
