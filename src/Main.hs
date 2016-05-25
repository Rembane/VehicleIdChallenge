{-# LANGUAGE OverloadedStrings #-}

import Data.Bits ((.&.), shiftR, setBit, testBit)
import qualified Data.ByteString as B
import qualified Data.Vector.Unboxed.Mutable as MU
import Data.Word (Word8)
import System.Environment (getArgs)
import System.IO.MMap (mmapFileByteString)

consumer :: MU.IOVector Word8 -> B.ByteString -> IO Bool
consumer r s | B.null s = return False
             | otherwise = do
               cell <- MU.read r shidx
               if cell `testBit` (7 .&. idx)
                  then return True
                  else do
                    MU.write r shidx (cell `setBit` (7 .&. idx))
                    consumer r rest
  where
    (s', rest) = B.splitAt 7 s
    idx = (((fromIntegral $ B.index s' 0) - 64)  * 676000 +
          (( fromIntegral $ B.index s' 1) - 64)  * 26000  +
          (( fromIntegral $ B.index s' 2) - 64)  * 1000   +
          (( fromIntegral $ B.index s' 3) - 48)  * 100    +
          (( fromIntegral $ B.index s' 4) - 48)  * 10     +
          (( fromIntegral $ B.index s' 5) - 48)) - 703111
    shidx = idx `shiftR` 3

main :: IO ()
main = do
  fn     <- (!! 0) `fmap` getArgs
  input  <- mmapFileByteString fn Nothing
  r      <- MU.replicate 2196986 0
  result <- consumer r input

  if result
     then putStrLn "Found duplicate!"
     else putStrLn "Nope."
